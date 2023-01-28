#ifndef TOKEN_H
#define TOKEN_H

#include "Position.h"

enum TokenType{
    HEADER_TYPE,
    INTEGER_TYPE,
    LABEL_TYPE,
    TEXT_TYPE
};

struct Token{
    char* Text;
    struct Position* Pos;
    enum TokenType Type;
};

struct TokenListElem{
    struct Token* Tok;
    struct TokenListElem* Next;
};

struct TokenList{
    struct TokenListElem* Root;
};

int TokenListIsEmpty(struct TokenList* Toks);
struct Token* RemoveToken(struct TokenList* Toks);
int AddToken(struct TokenList* Toks, struct Token* Tok);

void PrintTokenList(struct TokenList* List);
void PrintTokenType(enum TokenType Type);
void PrintToken(struct Token* Tok);

#endif