#include "Token.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void PrintTokenType(enum TokenType Type){
    if(Type == HEADER_TYPE){
        printf("HEADER_TYPE");
    } else if(Type == INTEGER_TYPE){
        printf("INTEGER_TYPE");
    } else if(Type == LABEL_TYPE){
        printf("LABEL_TYPE");
    } else if(Type == TEXT_TYPE){
        printf("TEXT_TYPE");
    } else {
        printf("Unknown Token Type With Number Value ~ %d", Type);
    }
}

void PrintToken(struct Token* Tok){
    printf("Tok { ");
    PrintTokenType(Tok->Type);
    printf(" | %s | ", Tok->Text);
    PrintPosition(Tok->Pos);
    printf(" }\n");
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

int TokenListIsEmpty(struct TokenList* Toks){
    return ((Toks != NULL) && (Toks->Root != NULL)) ? 0 : 1;
}

struct Token* RemoveToken(struct TokenList* Toks){
    if(!TokenListIsEmpty(Toks)){
        struct TokenListElem* ToRemoveElem = Toks->Root;
        Toks->Root = Toks->Root->Next;
        struct Token* Tok = ToRemoveElem->Tok;
        free(ToRemoveElem);
        return Tok;
    } else {
        return NULL:
    }
}

int AddToken(struct TokenList* Toks, struct Token* Tok){
    struct TokenListElem* Elem;
    for(Elem = Toks->Root; Elem->Next != NULL; Elem=Elem->Next);
    Elem->Next = malloc(sizeof(struct TokenListElem));
    Elem->Next->Tok = Tok;
    return 0;
}