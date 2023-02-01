#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <stdlib.h>
#include <string.h>

struct InputFile{
    char* FilePath;
};

struct OutputFile{
    char* FilePath;
};

struct CommandLine{
    struct InputFileList* Inputs;
    struct OutputFile* Output;
};

struct InputFileListElem{
    struct InputFile* File;
    struct InputFileListElem* Next;
};

struct InputFileList{
    struct InputFileListElem* Root;
};

struct CommandLine* ParseCommandLineData(int Argc, char** Argv);

#endif
