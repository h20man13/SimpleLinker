#ifndef TOKEN_H
#define TOKEN_H

enum TokenType{
    SEGMENTS,
    SYMBOLS,
    RELOCATIONS,
    DATA,
    INTEGER,
    LABEL,
};

struct Token{
    char* TokenText;
    Position Pos;
    TokenType Type;
}

struct TokenListElem{
    struct Token Tok;
    struct TokenListElem* Next;
}

struct TokenList{
    struct TokenListElem Root;
}


void PrintTokenList(struct TokenList* List);
void PrintTokenType(enum TokenType Type);
void PrintToken(struct Token* Tok);

#endif