#include "Symbol.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* SymbolTypeToString(enum SymbolType Type){
    if(Type == U) return "U";
    else return "D";
}

struct Symbol* GetSymbol(struct SymbolList* List, char* Name, enum SymbolType Type){
    for(struct SymbolEntry* Entry = List->Root; Entry != NULL; Entry=Entry->Next){
        if(strcmp(Name, Entry->Data->Name) == 0 && Entry->Data->Type == Type){
            return Entry->Data;
        }
    }
    return NULL;
}

int ContainsSymbol(struct SymbolList* List, char* Name, enum SymbolType Type){
    for(struct SymbolEntry* Entry = List->Root; Entry != NULL; Entry=Entry->Next){
        if(strcmp(Name, Entry->Data->Name) == 0 && Entry->Data->Type == Type){
            return 1;
        }
    }
    return 0;
}

void AddSymbol(struct SymbolList* List, struct Symbol* Sym){
    if(List->Root == NULL){
        List->Root = malloc(sizeof(struct SymbolEntry));
        List->Root->Data = Sym;
        List->Root->Next = NULL;
    } else {
        struct SymbolEntry* Entry = List->Root;
        List->Root = malloc(sizeof(struct SymbolEntry));
        List->Root->Data = Sym;
        List->Root->Next = Entry;
    }
}

void OverWriteSymbol(struct SymbolList* List, struct Symbol* Sym){
    for(struct SymbolEntry* Entry = List->Root; Entry != NULL; Entry=Entry->Next){
        if(strcmp(Entry->Data->Name, Sym->Name) == 0){
            Entry->Data = Sym;
        }
    }
}

int NumberOfSymbols(struct SymbolList* List){
    int Num = 0;
    for(struct SymbolEntry* Entry = List->Root; Entry != NULL; Entry=Entry->Next){
        Num++;
    }
    return Num;
}

struct Symbol* ToSymbolArray(struct SymbolList* List){
    int Amount = NumberOfSymbols(List);
    struct Symbol* Array = malloc(sizeof(struct Symbol) * Amount);
    struct SymbolEntry* Elem = List->Root;
    for(int i = 0; i < Amount; i++){
        memcpy(&(Array[i]), Elem->Data, sizeof(struct Symbol));
        Elem=Elem->Next;
    }
    return Array;
}