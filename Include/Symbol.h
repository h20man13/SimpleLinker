#ifndef SYMBOL_H
#define SYMBOL_H

enum SymbolType{
    U,
    D
};

char* SymbolTypeToString(enum SymbolType Type);

struct Symbol{
    char* Name;
    int Value;
    int SegmentNumber;
    enum SymbolType Type;
};

#endif
