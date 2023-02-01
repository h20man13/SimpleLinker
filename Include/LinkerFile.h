#ifndef LINKERFILE_H
#define LINKERFILE_H

struct LinkerFile{
    char MagicNumber[5]; //LINK
    int NumberSegments;
    int NumberSymbols;
    int NumberRelocationEntries;
    struct Segment* Segments;
    struct Symbol* Symbols;
    struct RelocationEntry* Entries;
};

struct LinkerFileListElem{
    struct LinkerFile* File;
    struct LinkerFileListElem* Next;
};

struct LinkerFileList{
    struct LinkerFileListElem* Root;
};

void PrintLinkerFile(struct LinkerFile*);

#endif 