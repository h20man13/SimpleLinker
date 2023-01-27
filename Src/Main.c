#include "LinkerFile.h"
#include "Parser.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_1_LEXER
#define TRACE

int main(int ArgC, char** Argv){
    if(ArgC <= 0){
        perror("Error: Not enough arguments found to run Simple Linker");
        return 1;
    }

#ifdef PROJECT_1
    //First we need to Parse the File
#ifdef TRACE
    printf("- Project 1 Output -\n");
#endif
    struct LinkerFile* FileStruct = malloc(sizeof(struct LinkerFile));

#ifdef TRACE 
    printf("Parsing input file - %s ...\n", Argv[1]);
#endif
    ParseLinkerFile(Argv[1], FileStruct);

#ifdef TRACE
    printf("Printing LinkerFile Output - %s ...\n", Argv[1]);
#endif

    PrintLinkerFile(FileStruct);
    
    free(FileStruct);
    
#endif

#ifdef PROJECT_1_LEXER
struct Lexer* Lex = malloc(sizeof(struct Lexer));

printf("Opening Lexer on File %s\n", Argv[1]);
int Result = OpenLexerFileSource(Lex, Argv[1], 60);
if(Result){
    perror("Error Opening Lexer the Session will time out");
    return 1;
}

printf("Lexing Tokens...");
struct TokenList* List;
Result = LexTokens(Lex, &List);
if(Result == 1){
    return 1;
}

PrintTokenList(List);

#endif
}