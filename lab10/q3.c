#include <stdio.h>

#define NUM_SEGMENTS 5

struct Segment {
    int base;   
    int limit;  
};

struct Segment segment_table[NUM_SEGMENTS] = {
    {1400, 1000},  
    {6300, 400},  
    {4300, 400},  
    {3200, 1100},  
    {4700, 1000}  
};

int logical_to_physical(int segment, int offset) {

    if (segment < 0 || segment >= NUM_SEGMENTS) {
        printf("Error: Invalid segment number %d\n", segment);
        return -1;
    }

    if (offset >= segment_table[segment].limit) {
        printf("Error: Offset %d is beyond the limit of segment %d\n", offset, segment);
        return -1;
    }

    int physical_address = segment_table[segment].base + offset;
    return physical_address;
}

int main() {

    // (i) 53 byte of segment 2
    int segment = 2;
    int offset = 53;
    int physical_address = logical_to_physical(segment, offset);
    if (physical_address != -1) {
        printf("Physical address of 53 byte in segment 2: %d\n", physical_address);
    }

    // (ii) 852 byte of segment 3
    segment = 3;
    offset = 852;
    physical_address = logical_to_physical(segment, offset);
    if (physical_address != -1) {
        printf("Physical address of 852 byte in segment 3: %d\n", physical_address);
    }

    // (iii) 1222 byte of segment 0
    segment = 0;
    offset = 1222;
    physical_address = logical_to_physical(segment, offset);
    if (physical_address != -1) {
        printf("Physical address of 1222 byte in segment 0: %d\n", physical_address);
    }

    return 0;
}