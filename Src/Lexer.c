#include "Lexer.h"

int OpenFileSource(struct Lexer* Lex, char* FileName){
    Lex->Source = fopen(FileName, "r");
    if(Lex->Source == NULL){
        return -1; //Error Source was not set correctly or some other error occured
    } else {
        return 0;
    }
}

char* LexerStateToString(enum LexerState State){
    if(State == INIT){
        return "INIT";
    } else if(State == HEADER){
        return "HEADER";
    } else if(State == INTEGER){
        return "INTEGER";
    } else if(State == LABEL){
        return "LABEL";
    } else if(State == TEXT){
        return "TEXT";
    } else if(State == WS){
        return "WS";
    } else if(State == COMMENT){
        return "COMMENT";
    } else {
        printf("Error Invalid State %d", State);
        return NULL;
    }
}

static char LexerGetChar(struct Lexer* Lex){
    if(Lex->File != NULL) return fgetc(Lex->Source);
    else return '\0';
}

static int LexerAppendLexeme(char CharToAppend, char*Lexeme, int IndexToAppend){
    Lexeme[IndexToAppend] = CharToAppend;
    IndexToAppend++;
}

int LexToken(struct Lexer* Lex, struct Token* Tok){
    if(Lex->Source == NULL){
        return -1;
    }

    char CurrentLexeme[40];
    int CurrentLexemeIndex = 0;
    enum LexerState CurrentState = INIT;
    static int LineNumber = 0;
    static int LinePosition = 0;

    while(1){
        char CurrentChar = LexerGetChar(Lex);
        switch (LexerState){
            case INIT:
                if(CurrentChar == '\r' || CurrentChar == '\n'){
                    LinePosition = 0;
                    LineNumber++;
                    CurrentState = WS;
                    continue;
                } else if(isspace(CurrentChar)){
                    CurrentState = WS;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '-'){
                    CurrentState = HEADER;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '['){
                    CurrentState = COMMENT;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '.'){
                    CurrentState = LABEL;
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else if(isletter(CurrentChar)){
                    CurrentState = TEXT;
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                } else {
                    printf("Invalid Chracter Found in Init Space - %c", CurrentChar);
                    continue;
                }
            case WS:
                if(CurrentChar == '\n'){
                    LineNumber++;
                    LinePosition=0;
                } else if(isspace(CurrentChar)){
                    LinePosition++;
                    continue;
                } else {
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                }
            case COMMENT:
                if(CurrentChar == ']'){
                    LinePosition++;
                    CurrentState = INIT;
                    continue;
                } else if(CurrentChar == '\n') {
                    LinePosition = 0;
                    LineNumber++;
                    continue;
                } else {
                    LinePosition++;
                    continue;
                }
            case HEADER:
                if (CurrentChar == '-'){
                    CurrentState = INIT;
                    Token Tok;
                    LinePosition++;
                    CreateTokenAndResetTempLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return Tok;
                } else if (CurrentChar == '\n'){
                    LinePosition = 0;
                    LineNumber++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                }
            case INTEGER:
                if(isdigit(CurrentChar) || (CurrentChar >= 'a' && CurrentChar <= 'f') || (CurrentChar >= 'A' && CurrentChar <= 'F')){
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    CurrentState = INIT;
                    Token Tok;
                    LinePosition++;
                    CreateTokenAndResetTempLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return Tok;
                }
            case LABEL:
                if(isspace(CurrentChar)){
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    CurrentState = INIT;
                    Token Tok;
                    LinePosition++;
                    CreateTokenAndResetTempLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return Tok;
                }
            case TEXT:
                if(!isspace()){
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    Token Tok;
                    LinePosition++;
                    CreateTokenAndResetTempLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return Tok;
                }
            default:
                printf("Error: In Invalid State %d", CurrentState);
                break;
        }
    }
    return NULL;
}

static int CreateTokenAndResetLexer(struct Token** Tok, int* LexemeIndex, char*LexemeArray, int LexemeArraySize, int* LineNumber, int* Position){
    *Tok = malloc(sizeof(struct Token));
    *(Tok)->Pos->LineNumber = *LineNumber;
    *(Tok)->Pos->LinePosition = *LinePosition;
    *(Tok)->TokenText = malloc(LexemeArraySize);
    memcpy(Tok->TokenText, LexemeArray, LexemeArraySize);

    //Now we need to reset some of the Temporary stuff...
    *LexemeIndex = 0;
    for(int i = 0; i < LexemeArraySize; i++){
        LexemeArray[i] = '\0';
    }
}