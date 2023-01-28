#ifndef RELOCATIONENTRY_H
#define RELOCATIONENTRY_H

enum RelocationType{
    A4,
    R4
};

char* RelocationTypeToString(enum RelocationType Type);

struct RelocationEntry{
    int Location;
    char* Seg;
    int Ref;
    enum RelocationType Type;
};

#endif