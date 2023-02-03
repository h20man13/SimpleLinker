#include "Segment.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "LinkerFile.h"

#define TRACE

char* SegmentFlagsToString(int FlagMask){
    char* FlagString = malloc(4);
    int index = 0;

    if((FlagMask & Readable)){
        FlagString[index++] = 'R';
    }
    
    if((FlagMask & Writable)){
        FlagString[index++] = 'W';
    }

    if((FlagMask & Present)){
        FlagString[index++] = 'P';
    }

    return FlagString;
}

int FlagStringToInt(char* FlagString){
    if(strcmp(FlagString, "RWP") == 0){
        return Readable | Writable | Present;
    } else if(strcmp(FlagString, "RW") == 0){
        return Readable | Writable;
    } else if(strcmp(FlagString, "WP") == 0){
        return Writable | Present;
    } else if(strcmp(FlagString, "RP") == 0){
        return Readable | Present;
    } else if(strcmp(FlagString, "R") == 0){
        return Readable;
    } else if(strcmp(FlagString, "W") == 0){
        return Writable;
    } else if(strcmp(FlagString, "P") == 0){
        return Present;
    } else {
        return -1;
    }
}

void AddSegment(struct SegmentList* List, struct Segment* Seg){
    if(List->Root == NULL){
        List->Root = malloc(sizeof(struct SegmentListElem));
        List->Root->Segment = Seg;
        List->Root->Next = NULL;
    } else {
        struct SegmentListElem* Holder = List->Root;
        List->Root = malloc(sizeof(struct SegmentListElem));
        List->Root->Segment = Seg;
        List->Root->Next = Holder;
    }
}

int ContainsSegment(struct SegmentList* List, char* SegmentName){
    for(struct SegmentListElem* Elem = List->Root; Elem != NULL; Elem=Elem->Next){
        if(strcmp(Elem->Segment->Name, SegmentName) == 0){
            return 1;
        }
    }

    return 0;
}

struct Segment* GetSegment(struct SegmentList* List, char* SegmentName){
    for(struct SegmentListElem* Elem = List->Root; Elem != NULL; Elem = Elem->Next){
        if(strcmp(Elem->Segment->Name, SegmentName) == 0){
            return Elem->Segment;
        }
    }
    return NULL;
}

void MergeSegment(struct SegmentList* List, struct Segment* Source){
    for(struct SegmentListElem* Elem = List->Root; Elem != NULL; Elem=Elem->Next){
        if(strcmp(Elem->Segment->Name, Source->Name) == 0){
            if(strcmp(Source->Name, ".common") == 0){
                //If we are merging a Common block we need to take the one with the Largest Size
                //If the one in the list has the largest size then we dont need to change it
                //however if the Segment we are merging contains the largest size then we need
                //to take that segment instead
                if(Source->NumberOfBytes > Elem->Segment->NumberOfBytes){
                    Elem->Segment->NumberOfBytes = Source->NumberOfBytes;
                }
            } else {
                //Otherwise add the sizes together to get the total size of the New Segment
                Elem->Segment->NumberOfBytes += Source->NumberOfBytes;
            }
            
            //We need to change Elem->Segments Flags so that they are the most restrictive out of the Merged Segments...
            //A bitwise and should allow for that to be the case
            Elem->Segment->Flags &= Source->Flags;

            //Finally since the Linker Links Ascii Text we need to Append all the Contents into the New Section
            Elem->Segment->ObjectData = strcat(Elem->Segment->ObjectData, Source->ObjectData);
            return;
        }
    }

    printf("Error: Cannot Merge Statement because there is no Statement with %s to merge into!!!", Source->Name);
}

int NumberOfSegments(struct SegmentList* List){
    int i = 0;
    for(struct SegmentListElem* Elem = List->Root; Elem != NULL; Elem=Elem->Next){
        i++;
    }
    return i;
}

struct Segment* ToSegmentArray(struct SegmentList* List){
    int Amount = NumberOfSegments(List);
    struct Segment* Array = malloc(sizeof(struct Segment) * Amount);
    struct SegmentListElem* Elem = List->Root;
    for(int i = 0; i < Amount; i++){
        memcpy(&(Array[i]), Elem->Segment, sizeof(struct Segment));
        Elem = Elem->Next;
    }
    return Array; 
}