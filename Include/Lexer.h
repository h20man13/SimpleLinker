
enum LexerState{
    INIT,
    HEADER,
    INTEGER,
    LABEL,
    TEXT,
    WS,
    COMMENT
}

char* LexerStateToString(enum LexerState State);

struct Lexer{
    FILE* Source;
    char CurrentCharacter;
}

int OpenLexerFileSource(struct Lexer* Lex, char* FileName);
int LexToken(struct Lexer* Lex, struct Token* Tok);
int LexTokens(struct Lexer* Lex, struct TokenList* Tok);
int EndLexer(struct Lexer* Lex);