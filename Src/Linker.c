#include "Linker.h"
#include "LinkerFile.h"
#include "Segment.h"
#include "Symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRACE

static struct SegmentList* AllocateStorage(struct LinkerFileList* Sources){
    //The First Pass we need to go through all the Source Segments and either
    //1) Add the Segment if it doesnt exists allready
    //2) Merge the Segment into an already existing Segment
    //After this process is done on all the Linker Files
    struct SegmentList* List = malloc(sizeof(struct SegmentList));
    List->Root = NULL;

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

static struct SymbolList* ResolveSymbols(struct LinkerFileList* Sources){
    struct SymbolList* List = malloc(sizeof(struct SymbolList));
    List->Root = NULL;

    for(struct LinkerFileListElem* Elem = Sources->Root; Elem != NULL; Elem=Elem->Next){
        for(int i = 0; i < Elem->File->NumberSymbols; i++){
            struct Symbol* Sym = &(Elem->File->Symbols[i]);
            if(Sym->Type == U){
                if(!ContainsSymbol(List, Sym->Name, U) && !ContainsSymbol(List, Sym->Name, D)){
                    //If there isnt a Defined Symbol and there isnt a Undefined Symbol we can add 
                    //the Undefined Symbol to the List
                    AddSymbol(List, Sym);
                }
            } else {
                //The Symbol Type is Defined and therefor should probably be added to the list
                if(ContainsSymbol(List, Sym->Name, D)){
                    //If the Symbol Table allready contains a Defined Symbol then we need to report an Error
                    printf("Symbol mismatch occured!!!\n the Symbol was allready defined somewhere else in the Program...\n");
                } else if(ContainsSymbol(List, Sym->Name, U)){
                    //If we contain an Undefined Symbol We need to OverWrite the Undefined Symbol with the Defined Symbol
                    OverWriteSymbol(List, Sym);
                } else {
                    //Otherwise we need to add the Symbol to the Corresponding List
                    AddSymbol(List, Sym);
                }
            }
        }
    }

    //Finnaly we need to still report any aymbols that are undefined up to this point
    for(struct SymbolEntry* Elem = List->Root; Elem != NULL; Elem=Elem->Next){
        if(Elem->Data->Type == U){
            printf("Error: Definition of Symbol %s is undefined and therefor will be left unresolved!!!\n", Elem->Data->Name);
        }
    }

    return List;
}

struct LinkerFile* Link(struct CommandLine* Command, struct LinkerFileList* Sources){
    //First we will go through all the Segments in the Target and modify both the Definitions and the Object Data
    //Associated with each of the Segments
    struct SegmentList* AllocatedSegments = AllocateStorage(Sources);
    struct SymbolList* ResolvedSymbols =  ResolveSymbols(Sources);

    int NumSegments = NumberOfSegments(AllocatedSegments);
    struct Segment* SegmentArray = ToSegmentArray(AllocatedSegments);

    int NumSymbols = NumberOfSymbols(ResolvedSymbols);
    struct Symbol* SymbolTable = ToSymbolArray(ResolvedSymbols);

    struct LinkerFile* OutputLinkerFile = malloc(sizeof(struct LinkerFile));
    memcpy(OutputLinkerFile->MagicNumber, "LINK", 4);
    OutputLinkerFile->NumberSegments = NumSegments;
    OutputLinkerFile->NumberSymbols = NumSymbols;
    OutputLinkerFile->NumberRelocationEntries = 0;
    OutputLinkerFile->Segments = SegmentArray;
    OutputLinkerFile->Symbols = SymbolTable;
    OutputLinkerFile->Entries = NULL;


    return OutputLinkerFile;
}