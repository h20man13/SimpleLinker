#include "LinkerFile.h"
#include "Parser.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_1_FULL
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
printf("More code before the entry of the function...\n");
int Result = OpenLexerFileSource(Lex, Argv[1], 100);
if(Result){
    perror("Error Opening Lexer the Session will time out");
    return 1;
}

struct TokenList* List = LexTokens(Lex);

printf("Printing Token List...");

PrintTokenList(List);

#endif

#ifdef PROJECT_1_FULL

struct Lexer* Lex = malloc(sizeof(struct Lexer));
int Result = OpenLexerFileSource(Lex, Argv[1], 100);
struct TokenList* List = LexTokens(Lex);

struct Parser* P = malloc(sizeof(struct Parser));
P->Tokens = List;

struct LinkerFile* LFile = ParseLinkerFile(P);
PrintLinkerFile(LFile);


#endif
}