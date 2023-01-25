#include "Segment.h"
#include <stdlib.h>

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