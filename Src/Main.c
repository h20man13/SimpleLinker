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

    struct CommandLine* CommandLine = ParseCommandLineData(ArgC, Argv);

    struct LinkerFileList* List = ParseLinkerFiles(CommandLine);
    //Now we have a List of Linker Files that were Parsed.
    //After they are Parsed we need to do the following
    //We need to Add all of the Segmnets to the Output Linker File.
    //All this is supported inside the Link Function

    struct LinkerFile* OutputFile = Link(List);

    WriteLinkerFileToOutput(OutputFile, CommandLine->Output->FilePath);

}