#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include <string.h>
#include <ctype.h>

#define TRACE

int OpenLexerFileSource(struct Lexer* Lex, char* FileName, int LexemeBufferSize){
    printf("Opening the file source...");
    printf("Printing data...");
    Lex->Source = fopen(FileName, "r");

    printf("Setting up initial variables...");
    Lex->CurrentCharacter = '\0';
    Lex->LexemeArraySize = LexemeBufferSize;
    Lex->LexemeIndex = 0;
    Lex->LineNumber = 0;
    Lex->LinePosition = 0;
    Lex->Lexeme = malloc(LexemeBufferSize);
    if(Lex->Source == NULL){
        printf("Error the File %s could not be opened successfully", FileName);
        return -1; //Error Source was not set correctly or some other error occured
    } else {
#ifdef TRACE
        printf("Advancing Source of the Lexer");
#endif
        AdvanceSource(Lex);
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
        return NULL;
    }
}

static int LexerAppendLexeme(struct Lexer* Lex){
    Lex->Lexeme[Lex->LexemeIndex] = PeekLetter(Lex);
    Lex->LexemeIndex++;
    return 0;
}

static int CreateTokenAndResetLexer(struct Token** Tok, struct Lexer* Lex){
    *Tok = malloc(sizeof(struct Token));
    (*Tok)->Pos.LineNumber = Lex->LineNumber;
    (*Tok)->Pos.LinePosition = Lex->LinePosition;
    (*Tok)->Text = malloc(Lex->LexemeArraySize);
    memcpy((*Tok)->Text, Lex->Lexeme, Lex->LexemeArraySize);

    //Now we need to reset some of the Temporary stuff...
    Lex->LexemeIndex = 0;
    for(int i = 0; i < Lex->LexemeArraySize; i++){
        Lex->Lexeme[i] = '\0';
    }

    return 0;
}

int LexToken(struct Lexer* Lex, struct Token* Tok){
    if(Lex->Source == NULL){
        return 0;
    }

    enum LexerState CurrentState = INIT_STATE;

    char CurrentChar;
    while((CurrentChar = PeekLetter(Lex))){
        switch (CurrentState){
            case INIT_STATE:
                if(isspace(CurrentChar)){
                    CurrentState = WS_STATE;
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '-'){
                    CurrentState = HEADER_STATE;
                    LexerAppendLexeme(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '['){
                    CurrentState = COMMENT_STATE;
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '.'){
                    CurrentState = LABEL_STATE;
                    LexerAppendLexeme(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar >= 'a' && CurrentChar <= 'z' || CurrentChar >= 'A' || CurrentChar <= 'Z'){
                    CurrentState = TEXT_STATE;
                    LexerAppendLexeme(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    printf("Invalid Chracter Found in Init Space - %c", CurrentChar);
                    break;
                }
            case WS_STATE:
                if(isspace(CurrentChar)){
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    continue;
                }
            case COMMENT_STATE:
                if(CurrentChar == ']'){
                    CurrentState = INIT_STATE;
                    AdvanceSource(Lex);
                    continue;
                } else {
                    AdvanceSource(Lex);
                    continue;
                }
            case HEADER_STATE:
                if (CurrentChar == '-'){
                    CurrentState = INIT_STATE;
                    CreateTokenAndResetLexer(&Tok, Lex);
                    return 1;
                } else {
                    LexerAppendLexeme(Lex);
                    continue;
                }
            case INTEGER_STATE:
                if(isdigit(CurrentChar) || (CurrentChar >= 'a' && CurrentChar <= 'f') || (CurrentChar >= 'A' && CurrentChar <= 'F')){
                    LexerAppendLexeme(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    CreateTokenAndResetLexer(&Tok, Lex);
                    return 1;
                }
            case LABEL_STATE:
                if(!isspace(CurrentChar)){
                    LexerAppendLexeme(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    CreateTokenAndResetLexer(&Tok, Lex);
                    return 1;
                }
            case TEXT_STATE:
                if(!isspace(CurrentChar)){
                    LexerAppendLexeme(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    CreateTokenAndResetLexer(&Tok, Lex);
                    return 1;
                }
            default:
                printf("Error: In Invalid State %d", CurrentState);
                break;
        }
    }
    return 0;
}

int LexTokens(struct Lexer* Lex, struct TokenList** TokList){
    #ifdef TRACE
        printf("Setting up the Array...");
    #endif 
    *TokList = malloc(sizeof(struct TokenList));
    struct TokenListElem** Elem = &((*TokList)->Root);
    struct Token* Tok;

    if(LexToken(Lex, Tok)){
        *Elem = malloc(sizeof(struct TokenListElem));
        (*Elem)->Tok = Tok;
        while(LexToken(Lex, Tok)){
            (*Elem)->Next = malloc(sizeof(struct TokenListElem));
            *Elem = (*Elem)->Next;
            (*Elem)->Tok = Tok;
        }
        return 1;
    }

    return 0;
}

char PeekLetter(struct Lexer* Lex){
    return Lex->CurrentCharacter;
}

char AdvanceSource(struct Lexer* Lex){
    Lex->CurrentCharacter = fgetc(Lex->Source);
    
    if(Lex->CurrentCharacter == '\n'){
        Lex->LineNumber++;
        Lex->LinePosition = 0;
    } else {
        Lex->LinePosition++;
    }

    return Lex->CurrentCharacter;
}