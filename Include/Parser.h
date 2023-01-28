#ifndef PARSER_H
#define PARSER_H

#include "LinkerFile.h"

enum ParserErrorStrategy{
    SKIP_STRATEGY,
    REPAIR_STRATEGY,
    ABORT_STRATEGY
};

struct Parser{
    struct TokenList* Tokens;
};

int Eat(struct Parser* P, enum TokenType T, enum ParserErrorStrategy Strat);
int ShouldEat(struct Parser* P, enum TokenType T);
int EatIfYummy(struct Parser* P, enum TokenType T);

struct LinkerFile* ParseLinkerFile(struct Parser* Context);
struct RelocationEntry* ParseRelocationEntry(struct Parser* Context);
struct Segment* ParseSegment(struct Parser* Context);
struct Symbol* ParseSymbol(struct Parser* Context);

int Parse


#endif