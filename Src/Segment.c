#include "Segment.h"
#include <stdlib.h>
#include <string.h>

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
    if(strcmp(FlagString, "RWP")){
        return Readable | Writable | Present;
    } else if(strcmp(FlagString, "RW")){
        return Readable | Writable;
    } else if(strcmp(FlagString, "WP")){
        return Writable | Present;
    } else if(strcmp(FlagString, "RP")){
        return Readable | Present;
    } else if(strcmp(FlagString, "R")){
        return Readable;
    } else if(strcmp(FlagString, "W")){
        return Writable;
    } else if(strcmp(FlagString, "P")){
        return Present;
    } else {
        return -1;
    }
}