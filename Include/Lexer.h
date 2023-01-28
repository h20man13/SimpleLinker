#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "Token.h"

enum LexerState{
    INIT_STATE,
    HEADER_STATE,
    INTEGER_STATE,
    LABEL_STATE,
    TEXT_STATE,
    WS_STATE,
    COMMENT_STATE
};

struct Lexer{
    FILE* Source;
    int LinePosition;
    int LineNumber;
    char CurrentCharacter;
    char* Lexeme;
    int LexemeIndex;
    int LexemeArraySize;
};

char* LexerStateToString(enum LexerState State);
char PeekLetter(struct Lexer* Lex);
char AdvanceSource(struct Lexer* Lex);

int OpenLexerFileSource(struct Lexer* Lex, char* FileName, int LexemeBufferSize);
struct Token* LexToken(struct Lexer* Lex);
struct TokenList* LexTokens(struct Lexer* Lex);
int EndLexer(struct Lexer* Lex);

#endif