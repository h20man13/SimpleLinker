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

        if(Seg != NULL){
            printf("%s %d %x %s\n", Seg->Name, Seg->NumberOfBytes, Seg->StartingAddress, SegmentFlagsToString(Seg->Flags));
        }
    }

    for(int i = 0; i < ToPrint->NumberSymbols; i++){
        struct Symbol* Sym = &(ToPrint->Symbols[i]);

        if(Sym != NULL){
            printf("%s %x %d %s\n", Sym->Name, Sym->Value, Sym->SegmentNumber, SymbolTypeToString(Sym->Type));
        }
    }

    for(int i = 0; i < ToPrint->NumberRelocationEntries; i++){
        struct RelocationEntry* Rel = &(ToPrint->Entries[i]);
        if(Rel != NULL){
            printf("%x %s %x %s\n", Rel->Location, Rel->Seg, Rel->Ref, RelocationTypeToString(Rel->Type));
        }
    }

    for(int i = 0; i < ToPrint->NumberSegments; i++){
        struct Segment* Seg = &(ToPrint->Segments[i]);
        if(Seg != NULL){
            printf("%s\n", Seg->ObjectData);
        }
    }
}

void WriteLinkerFileToOutput(struct LinkerFile* ToPrint, char* FileName){
    FILE* FilePtr = fopen(FileName, "w");
    fprintf(FilePtr, "%s\n", ToPrint->MagicNumber);
    for(int i = 0; i < ToPrint->NumberSegments; i++){
        struct Segment* Seg = &(ToPrint->Segments[i]);

        if(Seg != NULL){
            fprintf(FilePtr, "%s %d %x %s\n", Seg->Name, Seg->NumberOfBytes, Seg->StartingAddress, SegmentFlagsToString(Seg->Flags));
        }
    }

    for(int i = 0; i < ToPrint->NumberSymbols; i++){
        struct Symbol* Sym = &(ToPrint->Symbols[i]);

        if(Sym != NULL){
            fprintf(FilePtr, "%s %x %d %s\n", Sym->Name, Sym->Value, Sym->SegmentNumber, SymbolTypeToString(Sym->Type));
        }
    }

    for(int i = 0; i < ToPrint->NumberRelocationEntries; i++){
        struct RelocationEntry* Rel = &(ToPrint->Entries[i]);
        if(Rel != NULL){
            fprintf(FilePtr, "%x %s %x %s\n", Rel->Location, Rel->Seg, Rel->Ref, RelocationTypeToString(Rel->Type));
        }
    }

    for(int i = 0; i < ToPrint->NumberSegments; i++){
        struct Segment* Seg = &(ToPrint->Segments[i]);
        if(Seg != NULL){
            fprintf(FilePtr, "%s\n", Seg->ObjectData);
        }
    }
}