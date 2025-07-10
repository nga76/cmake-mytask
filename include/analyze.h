#ifndef ANALYZE_H
#define ANALYZE_H

typedef struct StatData {
    long id;
    int count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

int add(int a, int b);
int subtract(int a, int b);

void PrintDump(StatData *data, int size, int count);
int StoreDump(const char *filename, const StatData *data, int size);
StatData *LoadDump(const char *filename, int *size);

StatData *JoinDump(const StatData *data1, int size1, const StatData *data2, int size2, int *result_size);
StatData *SortDump(const StatData *data, int size);

#endif // ANALYZE_H
