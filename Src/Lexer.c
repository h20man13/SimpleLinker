#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include <string.h>

int OpenFileSource(struct Lexer* Lex, char* FileName){
    Lex->Source = fopen(FileName, "r");
    if(Lex->Source != NULL){
        return -1; //Error Source was not set correctly or some other error occured
    } else {
        return 0;
    }
}

char* LexerStateToString(enum LexerState State){
    if(State == INIT_STATE){
        return "INIT_STATE";
    } else if(State == HEADER_STATE){
        return "HEADER_STATE";
    } else if(State == INTEGER_STATE){
        return "INTEGER_STATE";
    } else if(State == LABEL_STATE){
        return "LABEL_STATE";
    } else if(State == TEXT_STATE){
        return "TEXT_STATE";
    } else if(State == WS_STATE){
        return "WS_STATE";
    } else if(State == COMMENT_STATE){
        return "COMMENT_STATE";
    } else {
        printf("Error Invalid State %d", State);
        return -1;
    }
}

static char LexerGetChar(struct Lexer* Lex){
    if(Lex->Source != 0) return fgetc(Lex->Source);
    else return '\0';
}

static int LexerAppendLexeme(char CharToAppend, char*Lexeme, int IndexToAppend){
    Lexeme[IndexToAppend] = CharToAppend;
    IndexToAppend++;
}

static int CreateTokenAndResetLexer(struct Token** Tok, int* LexemeIndex, char*LexemeArray, int LexemeArraySize, int* LineNumber, int* LinePosition){
    *Tok = malloc(sizeof(struct Token));
    (*Tok)->Pos.LineNumber = *LineNumber;
    (*Tok)->Pos.LinePosition = *LinePosition;
    (*Tok)->Text = malloc(LexemeArraySize);
    memcpy((*Tok)->Text, LexemeArray, LexemeArraySize);

    //Now we need to reset some of the Temporary stuff...
    *LexemeIndex = 0;
    for(int i = 0; i < LexemeArraySize; i++){
        LexemeArray[i] = '\0';
    }
}

int LexToken(struct Lexer* Lex, struct Token* Tok){
    if(Lex->Source == NULL){
        return -1;
    }

    char CurrentLexeme[40];
    int CurrentLexemeIndex = 0;
    enum LexerState CurrentState = INIT_STATE;
    static int LineNumber = 0;
    static int LinePosition = 0;

    char CurrentChar;
    while(CurrentChar = LexerGetChar(Lex)){
        switch (CurrentState){
            case INIT_STATE:
                if(CurrentChar == '\r' || CurrentChar == '\n'){
                    LinePosition = 0;
                    LineNumber++;
                    CurrentState = WS_STATE;
                    continue;
                } else if(isspace(CurrentChar)){
                    CurrentState = WS_STATE;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '-'){
                    CurrentState = HEADER_STATE;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '['){
                    CurrentState = COMMENT_STATE;
                    LinePosition++;
                    continue;
                } else if(CurrentChar == '.'){
                    CurrentState = LABEL_STATE;
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else if(CurrentChar >= 'a' && CurrentChar <= 'z' || CurrentChar >= 'A' || CurrentChar <= 'Z'){
                    CurrentState = TEXT_STATE;
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                } else {
                    printf("Invalid Chracter Found in Init Space - %c", CurrentChar);
                    break;
                }
            case WS_STATE:
                if(CurrentChar == '\n'){
                    LineNumber++;
                    LinePosition=0;
                    continue;
                } else if(isspace(CurrentChar)){
                    LinePosition++;
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                }
            case COMMENT_STATE:
                if(CurrentChar == ']'){
                    LinePosition++;
                    CurrentState = INIT_STATE;
                    continue;
                } else if(CurrentChar == '\n') {
                    LinePosition = 0;
                    LineNumber++;
                    continue;
                } else {
                    LinePosition++;
                    continue;
                }
            case HEADER_STATE:
                if (CurrentChar == '-'){
                    CurrentState = INIT_STATE;
                    LinePosition++;
                    CreateTokenAndResetLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return 0;
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
            case INTEGER_STATE:
                if(isdigit(CurrentChar) || (CurrentChar >= 'a' && CurrentChar <= 'f') || (CurrentChar >= 'A' && CurrentChar <= 'F')){
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    LinePosition++;
                    CreateTokenAndResetLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return 0;
                }
            case LABEL_STATE:
                if(isspace(CurrentChar)){
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    LinePosition++;
                    CreateTokenAndResetLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return 0;
                }
            case TEXT_STATE:
                if(!isspace(CurrentChar)){
                    LinePosition++;
                    LexerAppendLexeme(CurrentChar, CurrentLexeme, CurrentLexemeIndex);
                    continue;
                } else {
                    LinePosition++;
                    CreateTokenAndResetLexer(&Tok, &CurrentLexemeIndex, CurrentLexeme, 40, &LineNumber, &LinePosition);
                    return 0;
                }
            default:
                printf("Error: In Invalid State %d", CurrentState);
                break;
        }
    }
    return -1;
}