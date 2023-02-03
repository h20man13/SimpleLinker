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
    printf("Printing Out the Resulting LinkerFile...\n");
#endif
PrintLinkerFile(OutputFile);

}