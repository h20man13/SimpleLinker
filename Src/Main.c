#include "LinkerFile.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_1

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
}