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

struct SegmentListElem{
    struct Segment* Segment;
    struct SegmentListElem* Next;
};

struct SegmentList{
    struct SegmentListElem* Root;
};

void AddSegment(struct SegmentList* List, struct Segment* Seg);
int ContainsSegment(struct SegmentList* List, char* SegmentName);
struct Segment* GetSegment(struct SegmentList* List, char* SegmentName);
void MergeSegment(struct SegmentList* List, struct Segment* Seg);
int NumberOfSegments(struct SegmentList* List);
struct Segment* ToSegmentArray(struct SegmentList* List);


char* SegmentFlagsToString(int FlagMask);
int FlagStringToInt(char* FlagString);

#endif