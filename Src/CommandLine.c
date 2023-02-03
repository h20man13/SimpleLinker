#include "CommandLine.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define TRACE

struct CommandLine* ParseCommandLineData(int Argc, char** Argv){
    struct CommandLine* CommandLineConfig = malloc(sizeof(struct CommandLine));
    struct InputFileList* FileList = malloc(sizeof(struct InputFileList));
#ifdef TRACE
    printf("Parsing Argments...\n");
#endif
    for(int i = 1; i < Argc; i++){
        if(strcmp(Argv[i], "-o") == 0){
            if(i + 1 < Argc){
#ifdef TRACE
    printf("Parser Output...\n");
#endif
                i++;
                //We need to set the OutputFile
                CommandLineConfig->Output = malloc(sizeof(struct OutputFile));
                CommandLineConfig->Output->FilePath = Argv[i];
            }
        } else if(access(Argv[i], F_OK) == 0){
#ifdef TRACE
    printf("Command Line File Exists... %s\n", Argv[i]);
#endif
            if(FileList->Root == NULL){
                FileList->Root = malloc(sizeof(struct InputFileListElem));
                FileList->Root->File = malloc(sizeof(struct InputFile));
                FileList->Root->File->FilePath = Argv[i];
            } else {
                struct InputFileListElem* Holder = FileList->Root;
                FileList->Root = malloc(sizeof(struct InputFileListElem));
                FileList->Root->File = malloc(sizeof(struct InputFile));
                FileList->Root->File->FilePath = Argv[i];
                FileList->Root->Next = Holder;
            }
        } else {
            printf("No Input File Found at path: %s\n", Argv[i]);
        }
    }

    CommandLineConfig->Inputs = FileList;

#ifdef TRACE
    printf("Returning Command from CommandLine...\n");
#endif

    return CommandLineConfig;
}