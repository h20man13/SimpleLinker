#include "Parser.h"
#include <LinkerFile.h>
#include <Segment.h>
#include <Symbol.h>
#include "RelocationEntry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRACE

static int ParseFileMetadata(FILE* Ptr, struct LinkerFile* Output){
    int Result = fscanf(Ptr, "%x%*[ \t\n\r]%x%*[ \t\n\r]%x%*[ \n\r\t]", &(Output->NumberSegments), &(Output->NumberSymbols), &(Output->NumberRelocationEntries));
    if(Result != 3){
        perror("Error Could not Parse File Metadata!!!\n");
        return -1;
    }
    
    //Now we need to Use Malloc to allocate space for each of the corresponding items
    Output->Segments = malloc(sizeof(struct Segment) * Output->NumberSegments);
    Output->Symbols = malloc(sizeof(struct Symbol) * Output->NumberSymbols);
    Output->Entries = malloc(sizeof(struct RelocationEntry) * Output->NumberRelocationEntries);

    return 0;
}

static int ParseFileMagicNumber(FILE* Ptr, struct LinkerFile* Output){
    fgets(Output->MagicNumber, 5, Ptr);
    if(strcmp("LINK\n", Output->MagicNumber) == 0){
        perror("Magic Number is not equal to LINK!!!");
        return -1;
    }

    return 0;
}

static int ParseSegmentInformation(FILE* Ptr, struct LinkerFile* Output){
    for(int i = 0; i < Output->NumberSegments; i++){
        struct Segment* Seg = &(Output->Segments[i]);
        char* Str;
        int Result = fscanf(Ptr, "%*[ \t\n\r]%s%*[ \t\n\r]%x%*[ \t\n\r]%x%*[ \t\n\r]%s%*[ \t\n\r]", Seg->Name, &(Seg->NumberOfBytes), &(Seg->StartingAddress), Str);

        if(Result != 4){
            perror("Error: The Segment Information Did not Scan Correctly!!!");
            printf("Expected 4 arguments but only found %d\n", Result);
            return -1;
        }

        int ReadableChar = strchr(Str, 'R') != NULL;
        int WriteableChar = strchr(Str, 'W') != NULL;
        int PresentChar = strchr(Str, 'P') != NULL;

        if(ReadableChar) 
            Seg->Flags |= Readable;
        if(WriteableChar)
            Seg->Flags |= Writable;
        if(PresentChar)
            Seg->Flags |= Present;
    }

    return 0;
}

static int ParseSymbolInformation(FILE* Ptr, struct LinkerFile* Output){
    for(int i = 0; i < Output->NumberSymbols; i++){
        struct Symbol* Sym = &(Output->Symbols[i]);
        char* TypeStr;
        int Result = fscanf(Ptr, "%s%*[ \t\n\r]%x%*[ \t\n\r]%x%*[ \t\n\r]%s%*[ \t\n\r]", Sym->Name, &(Sym->Value), &(Sym->SegmentNumber), TypeStr);
        if(Result != 4){
            perror("fscanf failed reading Line!!!");
            return -1;
        }

        int IsU = strchr(TypeStr, 'U') != NULL;
        int IsD = strchr(TypeStr, 'D') != NULL;

        if(IsU != 0){
            Sym->Type = U;
        } else if(IsD != 0){
            Sym->Type = D;
        } else {
             perror("Invlid Type in TypeString!!!");
             return -1;
        }
    }

    return 0;
}

static int ParseRelocationInformation(FILE* Ptr, struct LinkerFile* Output){
    for(int i = 0; i < Output->NumberRelocationEntries; i++){
        struct RelocationEntry* Entry = &(Output->Entries[i]);
        char* TypeStr;
        int Result = fscanf(Ptr, "%x%*[ \t\n\r]%x%*[ \t\n\r]%x%*[ \t\n\r]%s%*[ \t\n\r]", &(Entry->Location), &(Entry->Seg), &(Entry->Ref), TypeStr);
        if(Result != 4){
            perror("Could not Parse Relocation Line!!!");
            return -1;
        }

        int IsAFour = strcmp("A4", TypeStr) == 0;
        int IsRFour = strcmp("R4", TypeStr) == 0;

        if(IsAFour){
            Entry->Type = A4;
        } else if(IsRFour){
            Entry->Type = R4;
        } else {
            perror("Invalid Type Found!!!");
            return -1;
        }
    }

    return 0;
}

static int ParseObjectData(FILE* Ptr, struct LinkerFile* Output){
    for(int i = 0; i < Output->NumberSegments; i++){
        Output->Segments[i].ObjectData = malloc(Output->Segments[i].NumberOfBytes * 2);
        int Result = fscanf(Ptr, "%s%*[ \t\n\r]", Output->Segments->ObjectData);
        if(Result != 1){
            perror("Error: when trying to scan object data!!!");
            return -1;
        }
    }

    return 0;
}

int ParseLinkerFile(const char* FileName, struct LinkerFile* Output){
#ifdef TRACE
    printf("Opening File at %s ...\n", FileName);
#endif
    FILE* FilePtr = fopen(FileName, "r");
    if(FilePtr == NULL){
      perror("Error opening file");
      return -1;
    }
#ifdef TRACE
    printf("Parsing File Magic Number...\n");
#endif
    //The First thing we need to do is read the Magic Number of the File
    int Result = ParseFileMagicNumber(FilePtr, Output);
    if(Result != 0){
        return 1;
    }
#ifdef TRACE
    printf("Parsing File Metadata...\n");
#endif
    //Next We need to Parse the File Metadata
    Result = ParseFileMetadata(FilePtr, Output);
    if(Result != 0){
        return 1;
    }
#ifdef TRACE
    //After the Metadata is Parsed we need to load the Segment information into memory
    printf("Parsing Segment Information...\n");
#endif
    Result = ParseSegmentInformation(FilePtr, Output);
    if(Result != 0){
        return 1;
    }
#ifdef TRACE
    printf("Parsing Symbol Information...\n");
#endif
    Result = ParseSymbolInformation(FilePtr, Output);
    if(Result != 0){
        return 1;
    }
#ifdef TRACE
    printf("Parsing Relocation Information...\n");
#endif
    Result = ParseRelocationInformation(FilePtr, Output);
    if(Result != 0){
        return 1;
    }
#ifdef TRACE
    printf("Parsing Object Data...\n");
#endif
    Result = ParseObjectData(FilePtr, Output);
    if(Result != 0){
        return 1;
    }

    fclose(FilePtr);

    return 0;
}