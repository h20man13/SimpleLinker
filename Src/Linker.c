#include "Linker.h"
#include "LinkerFile.h"
#include "Segment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct SegmentList* AllocateStorage(struct LinkerFileList* Sources){
    //The First Pass we need to go through all the Source Segments and either
    //1) Add the Segment if it doesnt exists allready
    //2) Merge the Segment into an already existing Segment
    //After this process is done on all the Linker Files

    struct SegmentList* List = malloc(sizeof(struct SegmentList));
    for(struct LinkerFileListElem* Elem = Sources->Root; Elem != NULL; Elem=Elem->Next){
        for(int i = 0; i < Elem->File->NumberSegments; i++){
            struct Segment* Seg = &(Elem->File->Segments[i]);
            if(ContainsSegment(List, Seg->Name)){
                MergeSegment(List, Seg);
            } else {
                AddSegment(List, Seg);
            }
        }
    }

    //Now after all the Elements have been Added/Merged Into the List
    //We need to Set all the New Starting Addresses of each Segment
    //This is done using the Starting Bytes Field
    int Start = ELF_STARTING_ADDRESS;
    for(struct SegmentListElem* Elem = List->Root; Elem != NULL; Elem=Elem->Next){
        Elem->Segment->StartingAddress = Start;
        Start += Elem->Segment->NumberOfBytes;
    }

    return List;
}

struct LinkerFile* Link(struct CommandLine* Command, struct LinkerFileList* Sources){
    //First we will go through all the Segments in the Target and modify both the Definitions and the Object Data
    //Associated with each of the Segments
    struct SegmentList* AllocatedSegments = AllocateStorage(Sources);

    int NumSegments = NumberOfSegments(AllocatedSegments);
    struct Segment* SegmentArray = ToSegmentArray(AllocatedSegments);

    struct LinkerFile* OutputLinkerFile = malloc(sizeof(struct LinkerFile));
    memcpy(OutputLinkerFile->MagicNumber, "LINK", 5);
    OutputLinkerFile->Segments = SegmentArray;
    OutputLinkerFile->NumberSegments = NumSegments;

    return OutputLinkerFile;
}