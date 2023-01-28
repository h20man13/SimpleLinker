#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include <string.h>
#include <ctype.h>

#define TRACE

int OpenLexerFileSource(struct Lexer* Lex, char* FileName, int LexemeBufferSize){
    Lex->Source = fopen(FileName, "r");
    Lex->LexemeArraySize = LexemeBufferSize;
    Lex->LexemeIndex = 0;
    Lex->LineNumber = 0;
    Lex->LinePosition = 0;
    Lex->Lexeme = malloc(LexemeBufferSize);
    if(Lex->Source == NULL){
        printf("Error the File %s could not be opened successfully\n", FileName);
        return -1; //Error Source was not set correctly or some other error occured
    } else {
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

static int LexerAppendCharacter(struct Lexer* Lex){
    Lex->Lexeme[Lex->LexemeIndex] = PeekLetter(Lex);
    Lex->LexemeIndex++;
    return 0;
}

static int ResetLexer(struct Lexer* Lex){
    Lex->LexemeIndex = 0;
    for(int i = 0; i < Lex->LexemeArraySize; i++){
        Lex->Lexeme[i] = '\0';
    }
    return 0;
}

static struct Token* CreateTokenAndResetLexer(struct Lexer* Lex, enum TokenType Type){
    struct Token* Tok = malloc(sizeof(struct Token));
    Tok->Pos = malloc(sizeof(struct Position));
    Tok->Pos->LineNumber = Lex->LineNumber;
    Tok->Pos->LinePosition = Lex->LinePosition;
    Tok->Text = malloc(Lex->LexemeArraySize);
    memcpy(Tok->Text, Lex->Lexeme, Lex->LexemeArraySize);
    Tok->Type = Type;
    
    ResetLexer(Lex);

    return Tok;
}

struct Token* LexToken(struct Lexer* Lex){
    if(Lex->Source == NULL){
        printf("Error: Lexrer Source was not ititialized correctly!!!\n");
        return NULL;
    }

    enum LexerState CurrentState = INIT_STATE;

    char CurrentChar;
    while((CurrentChar = PeekLetter(Lex)) && !feof(Lex->Source)){
        switch (CurrentState){
            case INIT_STATE:
                if(isspace(CurrentChar)){
                    CurrentState = WS_STATE;
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '-'){
                    CurrentState = HEADER_STATE;
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '['){
                    CurrentState = COMMENT_STATE;
                    AdvanceSource(Lex);
                    continue;
                } else if(CurrentChar == '.'){
                    CurrentState = LABEL_STATE;
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if (isdigit(CurrentChar) || (CurrentChar >= 'a' && CurrentChar <= 'f') || (CurrentChar >= 'A' && CurrentChar <= 'F')) {
                    CurrentState = INTEGER_STATE;
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if((CurrentChar >= 'a' && CurrentChar <= 'z') || (CurrentChar >= 'A' && CurrentChar <= 'Z')){
                    CurrentState = TEXT_STATE;
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    printf("Invalid Chracter Found in Init Space - %c\n", CurrentChar);
                    AdvanceSource(Lex);
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
                    LexerAppendCharacter(Lex);
                    return CreateTokenAndResetLexer(Lex, HEADER_TYPE);
                } else {
                    LexerAppendCharacter(Lex);
                    continue;
                }
            case INTEGER_STATE:
                if(isdigit(CurrentChar) || (CurrentChar >= 'a' && CurrentChar <= 'f') || (CurrentChar >= 'A' && CurrentChar <= 'F')){
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else if((CurrentChar >= 'g' && CurrentChar <= 'z') || (CurrentChar >= 'G' && CurrentChar <= 'Z')){
                    CurrentState = TEXT_STATE;
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    return CreateTokenAndResetLexer(Lex, INTEGER_TYPE);
                }
            case LABEL_STATE:
                if(!isspace(CurrentChar)){
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    return CreateTokenAndResetLexer(Lex, LABEL_TYPE);
                }
            case TEXT_STATE:
                if(!isspace(CurrentChar)){
                    LexerAppendCharacter(Lex);
                    AdvanceSource(Lex);
                    continue;
                } else {
                    CurrentState = INIT_STATE;
                    return CreateTokenAndResetLexer(Lex, TEXT_TYPE);
                }
            default:
                printf("Error: In Invalid State %d\n", CurrentState);
                break;
        }
    }
    return NULL;
}

struct TokenList* LexTokens(struct Lexer* Lex){
    struct TokenList* TokList = malloc(sizeof(struct TokenList));
    struct Token* Tok;
    if(!feof(Lex->Source)){
        if(Tok = LexToken(Lex)){
            struct TokenListElem* Elem = malloc(sizeof(struct TokenListElem));
            TokList->Root = Elem;
            Elem->Tok = Tok;
            while(Tok = LexToken(Lex)){
                Elem->Next = malloc(sizeof(struct TokenListElem));
                Elem = Elem->Next;
                Elem->Tok = Tok;

                if(feof(Lex->Source))
                    break;
            }
        } else {
            printf("Token did not lex correctly!!!");
        }
    } else {
        printf("Could not collect tokens from the file because the file was empty!!!");
    }

    return TokList;
}

char PeekLetter(struct Lexer* Lex){
    return Lex->CurrentCharacter;
}

char AdvanceSource(struct Lexer* Lex){
    if(!feof(Lex->Source)){
        Lex->CurrentCharacter = fgetc(Lex->Source);
        if(Lex->CurrentCharacter == '\n'){
            Lex->LineNumber++;
            Lex->LinePosition = 0;
        } else {
            Lex->LinePosition++;
        }
    } else {
        printf("Error: can not Advance Source that is allready at the end of the file!!!");
    }
    
    return Lex->CurrentCharacter;
}