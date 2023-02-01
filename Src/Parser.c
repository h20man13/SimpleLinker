#include "Parser.h"
#include <LinkerFile.h>
#include <Segment.h>
#include <Symbol.h>
#include "RelocationEntry.h"
#include "Token.h"
#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Token* Eat(struct Parser* P, enum TokenType T, enum ParserErrorStrategy Strategy){
    #ifdef TRACE 
        printf("Checking if Token is empty!!!");
    #endif
    if(TokenListIsEmpty(P->Tokens)){
        printf("Cannot Eat Token because the Token List Is Empty\n...");
        return NULL; 
    }

    if(P->Tokens->Root->Tok->Type == T){
        #ifdef TRACE
        printf("TokenMatch is a success...\n");
        #endif
        return RemoveToken(P->Tokens);
    } else {
        if(Strategy == SKIP_STRATEGY){
            struct Token* Tok = RemoveToken(P->Tokens);
            printf("Error: Token \n");
            PrintToken(Tok);
            printf("is not of type ");
            PrintTokenType(T);
            printf("!!!\n");
            return NULL;
        } else if(Strategy == REPAIR_STRATEGY){
            printf("Error: Token \n");
            PrintToken(P->Tokens->Root->Tok);
            printf("is not of type ");
            PrintTokenType(T);
            printf("!!!\n");
            return NULL;
        } else {
            PrintToken(P->Tokens->Root->Tok);
            printf("is not of type ");
            PrintTokenType(T);
            printf("!!!\n");
            exit(1);
            return NULL;
        }
    }
}

int ShouldEat(struct Parser* P, enum TokenType T){
    if(TokenListIsEmpty(P->Tokens)){
        return 0;
    }

    return P->Tokens->Root->Tok->Type == T;
}

struct Token* EatIfYummy(struct Parser* P, enum TokenType T){
    if(ShouldEat(P, T)){
        return Eat(P, T, ABORT_STRATEGY);
    }
    return NULL;
}

struct LinkerFileList* ParseLinkerFiles(struct CommandLine* Command){
    struct LinkerFileList* List = malloc(sizeof(struct LinkerFileList));
    for(struct InputFileListElem* Elem = Command->Inputs->Root; Elem != NULL; Elem = Elem->Next){
        struct Lexer* Lex = malloc(sizeof(struct Lexer));
        int Result = OpenLexerFileSource(Lex, Elem->File->FilePath, 100);
        struct TokenList* TokList = LexTokens(Lex);

        struct Parser* P = malloc(sizeof(struct Parser));
        P->Tokens = TokList;
    
        struct LinkerFile* LFile = ParseLinkerFile(P);
        if(List->Root == NULL){
            List->Root = malloc(sizeof(struct LinkerFileListElem));
            List->Root->File = LFile;
            List->Root->Next = NULL;
        } else {
            struct LinkerFileListElem* ListElem = List->Root;
            List->Root = malloc(sizeof(struct LinkerFileListElem));
            List->Root->File = LFile;
            List->Root->Next = ListElem;
        }
    }
}

struct LinkerFile* ParseLinkerFile(struct Parser* Context){
#ifdef TRACE
    printf("Initializing Magic Number...\n");
#endif
    //First we need to Parse the Magic Number
    struct LinkerFile* File = malloc(sizeof(struct LinkerFile));
    struct Token* Tok = Eat(Context, TEXT_TYPE, ABORT_STRATEGY);
    memcpy(File->MagicNumber, Tok->Text, 4);

#ifdef TRACE
    printf("Initializing Number of Segments...\n");
#endif
    
    //Now we need to Parse the Next 3 Header Numbers 
    //that say how many entries of each type that is in the file
    Tok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    File->NumberSegments = atoi(Tok->Text);
    File->Segments = malloc(sizeof(struct Segment) * File->NumberSegments);

#ifdef TRACE
    printf("Initializing Number of Symbols...\n");
#endif

    Tok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    File->NumberSymbols = atoi(Tok->Text);
    File->Symbols = malloc(sizeof(struct Symbol) * File->NumberSymbols);

#ifdef TRACE
    printf("Initializing Number of Relocaton Entries...\n");
#endif

    Tok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    File->NumberRelocationEntries = atoi(Tok->Text);
    File->Entries = malloc(sizeof(struct RelocationEntry) * File->NumberRelocationEntries);

    //After this we need to parse the following Sections based on their input type
#ifdef TRACE
    printf("Parsing Segment Definitions...\n");
#endif

    for(int i = 0; i < File->NumberSegments; i++){
        struct Segment* Seg = ParseSegment(Context);
        memcpy(File->Segments + i, Seg, sizeof(struct Segment));
    }

    //After this we need to Parse the Symbols that are utilized in the ABI
#ifdef TRACE
    printf("Parsing Symbol Definitions...\n");
#endif
    for(int i = 0; i < File->NumberSegments; i++){
        struct Symbol* Sym = ParseSymbol(Context);
        memcpy(File->Symbols + i, Sym, sizeof(struct Symbol));
    }

#ifdef TRACE
    printf("Parsing Relocation Entries...\n");
#endif
    //After this then comes the Relocation entries
    for(int i = 0; i < File->NumberRelocationEntries; i++){
        struct RelocationEntry* Entry = ParseRelocationEntry(Context);
        memcpy(File->Entries + i, Entry, sizeof(struct RelocationEntry));
    }

#ifdef TRACE
    //Now finally comes the Object Data
    printf("Parsing Object Data...\n");
#endif
    for(int i = 0; i < File->NumberSegments; i++){
        struct Segment* Seg = File->Segments + i;
        Tok = Eat(Context, INTEGER_TYPE, SKIP_STRATEGY);
        Seg->ObjectData = Tok->Text;
    }

    return File;
}

struct RelocationEntry* ParseRelocationEntry(struct Parser* Context){
    struct Token* LocationTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    struct Token* SegTok = Eat(Context, LABEL_TYPE, ABORT_STRATEGY);
    struct Token* RefTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    
    struct Token* RelTypeTok;
    if(ShouldEat(Context, INTEGER_TYPE)) RelTypeTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    else RelTypeTok = Eat(Context, TEXT_TYPE, ABORT_STRATEGY);
    
    struct RelocationEntry* Entry = malloc(sizeof(struct RelocationEntry));
    Entry->Location = atoi(LocationTok->Text);
    Entry->Seg = SegTok->Text;
    Entry->Ref = RefTok->Text;
    Entry->Type = (strcmp(RelTypeTok->Text, "R4")) ? R4 : A4;

    return Entry;
}

struct Segment* ParseSegment(struct Parser* Context){
    struct Token* LabelTok = Eat(Context, LABEL_TYPE, ABORT_STRATEGY);
    struct Token* LengthTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    struct Token* AddressTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    struct Token* FlagTok = Eat(Context, TEXT_TYPE, ABORT_STRATEGY);

    struct Segment* Result = malloc(sizeof(struct Segment));
    Result->Name = LabelTok->Text;
    Result->NumberOfBytes = atoi(LengthTok->Text);
    Result->StartingAddress = atoi(AddressTok->Text);
    Result->Flags = FlagStringToInt(FlagTok->Text);

    return Result;
}

struct Symbol* ParseSymbol(struct Parser* Context){
    struct Token* SymbolTok = Eat(Context, TEXT_TYPE, ABORT_STRATEGY);
    struct Token* SymbolValueTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    struct Token* SegmentNumberTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    
    struct Token* DefinedOrUndefinedTok;
    if(ShouldEat(Context, INTEGER_TYPE)) DefinedOrUndefinedTok = Eat(Context, INTEGER_TYPE, ABORT_STRATEGY);
    else DefinedOrUndefinedTok = Eat(Context, TEXT_TYPE, ABORT_STRATEGY);

    struct Symbol* Symbol = malloc(sizeof(struct Symbol));
    Symbol->Name = SymbolTok->Text;
    Symbol->Value = atoi(SymbolValueTok->Text);
    Symbol->SegmentNumber = atoi(SegmentNumberTok->Text);
    Symbol->Type == (strcmp(DefinedOrUndefinedTok->Text, "D")) ? D : U;

    return Symbol;
}