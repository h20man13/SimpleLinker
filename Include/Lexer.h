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

char* LexerStateToString(enum LexerState State);

struct Lexer{
    FILE* Source;
};

int OpenLexerFileSource(struct Lexer* Lex, char* FileName);
int LexToken(struct Lexer* Lex, struct Token* Tok);
int LexTokens(struct Lexer* Lex, struct TokenList* Tok);
int EndLexer(struct Lexer* Lex);

#endif