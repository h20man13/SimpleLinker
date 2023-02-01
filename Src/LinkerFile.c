#include "LinkerFile.h"
#include "Segment.h"
#include "Symbol.h"
#include "RelocationEntry.h"
#include <stdio.h>
#include <string.h>


void PrintLinkerFile(struct LinkerFile* ToPrint){
    //First we Print LINK
    printf("%s\n", ToPrint->MagicNumber);
    for(int i = 0; i < ToPrint->NumberSegments; i++){
        struct Segment* Seg = &(ToPrint->Segments[i]);
        printf("%s %d %x %s\n", Seg->Name, Seg->NumberOfBytes, Seg->StartingAddress, SegmentFlagsToString(Seg->Flags));
    }

    for(int i = 0; i < ToPrint->NumberSymbols; i++){
        struct Symbol* Sym = &(ToPrint->Symbols[i]);
        printf("%s %x %d %s\n", Sym->Name, Sym->Value, Sym->SegmentNumber, SymbolTypeToString(Sym->Type));
    }

    for(int i = 0; i < ToPrint->NumberRelocationEntries; i++){
        struct RelocationEntry* Rel = &(ToPrint->Entries[i]);
        printf("%x %s %x %s\n", Rel->Location, Rel->Seg, Rel->Ref, RelocationTypeToString(Rel->Type));
    }

    for(int i = 0; i < ToPrint->NumberSegments; i++){
        struct Segment* Seg = &(ToPrint->Segments[i]);
        printf("%s\n", Seg->ObjectData);
    }
}