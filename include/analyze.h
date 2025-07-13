#ifndef ANALYZE_H
#define ANALYZE_H

typedef struct StatData {
    long id;
    int count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

char* get_filename_from_path(const char* path);
void PrintDump(const StatData *data, size_t size, size_t count);
int StoreDump(const char *filename, const StatData *data, size_t size);
StatData *LoadDump(const char *filename, size_t *size);

StatData *JoinDump(const StatData *data1, size_t size1, const StatData *data2, size_t size2, size_t *result_size );
void SortDump(StatData *data, size_t size);

#endif // ANALYZE_H
