#ifndef PARSER_H
#define PARSER_H

#include "LinkerFile.h"
#include "Token.h"

enum ParserErrorStrategy{
    SKIP_STRATEGY,
    REPAIR_STRATEGY,
    ABORT_STRATEGY
};

struct Parser{
    struct TokenList* Tokens;
};

struct Token* Eat(struct Parser* P, enum TokenType T, enum ParserErrorStrategy Strategy);
int ShouldEat(struct Parser* P, enum TokenType T);
struct Token* EatIfYummy(struct Parser* P, enum TokenType T);

struct LinkerFile* ParseLinkerFile(struct Parser* Context);
struct RelocationEntry* ParseRelocationEntry(struct Parser* Context);
struct Segment* ParseSegment(struct Parser* Context);
struct Symbol* ParseSymbol(struct Parser* Context);


#endif