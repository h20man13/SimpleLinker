#import "Symbol.h"

char* SymbolTypeToString(enum SymbolType Type){
    if(Type == U) return "U";
    else return "D";
}