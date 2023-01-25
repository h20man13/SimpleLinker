#ifndef SEGMENT_H
#define SEGMENT_H

#define Readable 1
#define Writable 2
#define Present 4 

struct Segment{
    char* Name;
    int NumberOfBytes;
    int StartingAddress;
    int Flags;
    char* ObjectData;
};

char* SegmentFlagsToString(int FlagMask);

#endif