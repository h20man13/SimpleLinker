#include "Parser.h"
#include <LinkerFile.h>
#include <Segment.h>
#include <Symbol.h>
#include "RelocationEntry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRACE

int Eat(struct Parser* P, enum TokenType T, enum ParserErrorStrategy Strat){
    
}
int ShouldEat(struct Parser* P, enum TokenType T);
int EatIfYummy(struct Parser* P, enum TokenType T);

struct LinkerFile* ParseLinkerFile(struct Parser* Context);
struct RelocationEntry* ParseRelocationEntry(struct Parser* Context);
struct Segment* ParseSegment(struct Parser* Context);
struct Symbol* ParseSymbol(struct Parser* Context);