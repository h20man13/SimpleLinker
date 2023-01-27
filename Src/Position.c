#include "Position.h"

#include "stdio.h"

void PrintPosition(struct Position Pos){
    printf("[Line Number %d | Line Position %d]", Pos.LineNumber, Pos.LinePosition);
}