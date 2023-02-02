#ifndef SYMBOL_H
#define SYMBOL_H

enum SymbolType{
    U,
    D
};

struct Symbol{
    char* Name;
    int Value;
    int SegmentNumber;
    enum SymbolType Type;
};

struct SymbolEntry{
    struct Symbol* Data;
    struct SymbolEntry* Next;
};

struct SymbolList{
    struct SymbolEntry* Root;
};

char* SymbolTypeToString(enum SymbolType Type);
struct Symbol* GetSymbol(struct SymbolList* List, char* Name, enum SymbolType Type);
int ContainsSymbol(struct SymbolList* List, char* SymbolName, enum SymbolType Type);
void AddSymbol(struct SymbolList* List, struct Symbol* Sym);
void OverWriteSymbol(struct SymbolList* List, struct Symbol* Sym);
int NumberOfSymbols(struct SymbolList* List);
struct Symbol* ToSymbolArray(struct SymbolList* List);
#endif
