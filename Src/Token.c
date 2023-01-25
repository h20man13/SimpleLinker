#include "Token.h"

void PrintTokenType(enum TokenType Type){
    if(Type == SEGMENTS){
        Printf("- segments -");
    } else if(Type == SYMBOLS){
        Printf("- symbols -");
    } else if(Tok->Type == RELOCATIONS){
        Printf("- rels -");
    } else if(Tok->Type == DATA){
        Printf("- data -");
    } else if(Tok->Type == INTEGER){
        Printf("INTEGER");
    } else if(Tok->Type == LABEL){
        Printf("LABEL");
    } else if(Tok->Type == IDENTIIFER){
        Printf("IDENTIFIER");
    } else {
        perror("Unknown Token Type With Number Value ~ %d", Type);
    }
}
void PrintToken(struct Token* Tok);{
    Printf("Tok { ");
    PrintTokenType(Tok->Type);
    Printf("| %s | ", TokenText);
    PrintPosition(Tok->Pos);
    Printf(" }");
}

void PrintTokenList(struct TokenList List){
    struct TokenListElem* Elem = List->Root;
    if(Root != NULL){
        do{
            PrintToken(Elem->Token);
            Elem = Elem -> Next;
        } while(Elem != NULL);
    }
}