#include "RelocationEntry.h"

char* RelocationTypeToString(enum RelocationType Type){
    if(Type == A4) return "A4";
    else return "R4";
}