#include "Token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PrintTokenType(enum TokenType Type){
    if(Type == SEGMENTS){
        printf("- segments -");
    } else if(Type == SYMBOLS){
        printf("- symbols -");
    } else if(Type == RELOCATIONS){
        printf("- rels -");
    } else if(Type == DATA){
        printf("- data -");
    } else if(Type == INTEGER){
        printf("INTEGER");
    } else if(Type == LABEL){
        printf("LABEL");
    } else if(Type == DATA){
        printf("DATA");
    } else {
        printf("Unknown Token Type With Number Value ~ %d", Type);
    }
}

void PrintToken(struct Token* Tok){
    printf("Tok { ");
    PrintTokenType(Tok->Type);
    printf("| %s | ", Tok->Text);
    PrintPosition(Tok->Pos);
    printf(" }");
}

void PrintTokenList(struct TokenList* List){
    struct TokenListElem* Elem = List->Root;
    if(Elem != NULL){
        do{
            PrintToken(Elem->Tok);
            Elem = Elem->Next;
        } while(Elem != NULL);
    }
}