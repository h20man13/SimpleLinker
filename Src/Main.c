#include "LinkerFile.h"
#include "Parser.h"
#include "Lexer.h"
#include "Linker.h"
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

#endif

#ifdef PROJECT_1_FULL

#ifdef TRACE
    printf("Parsing Command Line Data...\n");
#endif 
struct CommandLine* CommandLine = ParseCommandLineData(ArgC, Argv);

#ifdef TRACE
    printf("Parsing all the LinkerFiles...\n");
#endif
struct LinkerFileList* List = ParseLinkerFiles(CommandLine);
//Now we have a List of Linker Files that were Parsed.
//After they are Parsed we need to do the following
//We need to Add all of the Segmnets to the Output Linker File.
//All this is supported inside the Link Function

#ifdef TRACE
    printf("Executing the Run Time Linker...\n");
#endif
struct LinkerFile* OutputFile = Link(CommandLine, List);

#ifdef TRACE
    printf("Printing Out the Resulting LinkerFile...");
#endif
PrintLinkerFile(OutputFile);

#endif
}