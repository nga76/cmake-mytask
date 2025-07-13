#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "analyze.h"

void PrintDump(const StatData *data, size_t size, size_t count){
    
    if (count > size) count = size;

    for (const StatData *ptr = data; ptr < data+count; ptr++)
        printf("| id = %08lX | count = %5d | cost = %.3e | primary = %s | mode = %c%c%c |\n",
             ptr->id, ptr->count, ptr->cost, ptr->primary == 0 ? "n" : "y",
             ptr->mode & 4 ? '1': '0', ptr->mode & 2 ? '1': '0', ptr->mode & 1 ? '1': '0');
}

int StoreDump(const char *filename, const StatData *data, size_t size){
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error: can't open file");
        return errno;
    }

    // write size of data
    fwrite(&size, sizeof(size_t), 1, file);
    
    // write data
    for (size_t i = 0; i < size; i++) {
        fwrite(&data[i], sizeof(StatData), 1, file);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

StatData *LoadDump(const char *filename, size_t *size){
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error: open file to read");
        return NULL;
    }

    // Read data size
    if (fread(size, sizeof(size_t), 1, file) != 1) {
        perror("Error: while read size of data");
        fclose(file);
        return NULL;
    }

    // Allocate memory
    StatData *data = (StatData *)malloc(*size * sizeof(StatData));
    if (data == NULL) {
        perror("Error: memory allocate");
        fclose(file);
        return NULL;
    }

    // Read data
    if (fread(data, sizeof(StatData), *size, file) != *size) {
        perror("Error: read data");
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}

int comp_id(const void *a, const void *b) {
    if ( (*(StatData **)(a))->id > (*(StatData **)(b))->id) return 1;
    else if( (*(StatData **)(a))->id < (*(StatData **)(b))->id) return -1;
    else return 0;
}


StatData *JoinDump(const StatData *data1, size_t size1, const StatData *data2, size_t size2, size_t *result_size ){
    StatData const **mergedPtrDump = (StatData const **)malloc((size1 + size2) * sizeof(StatData*));
    StatData *mergedDump = (StatData*)malloc((size1 + size2) * sizeof(StatData));
    if (!mergedDump) {
        perror("Error: memory allocate");
        return NULL;
    }
    if (!mergedPtrDump) {
        perror("Error: memory allocate");
        return NULL;
    }

    StatData const **mergedPtr = mergedPtrDump;
    for(StatData const *ptr = data1; ptr < data1+size1; ptr++, mergedPtr++) {
        *mergedPtr = ptr;
    }
    for(StatData const *ptr = data2; ptr < data2+size2; ptr++, mergedPtr++) {
        *mergedPtr = ptr;
    }
    
    qsort(mergedPtrDump, size1 + size2, sizeof(StatData*), comp_id);
    
    memcpy(mergedDump, *mergedPtrDump, sizeof(StatData));
    StatData *ptr = mergedDump + 1;
    mergedPtr = mergedPtrDump + 1;
    while(mergedPtr < mergedPtrDump + size1 + size2 && ptr < mergedDump + size1 + size2) {
        if ( (ptr - 1)->id == (*(mergedPtr))->id ) {
            (ptr - 1)->count += (*(mergedPtr))->count;
            (ptr - 1)->cost += (*(mergedPtr))->cost;
            (ptr - 1)->primary &= (*(mergedPtr))->primary;
            if ( (ptr - 1)->mode < (*(mergedPtr))->mode ) (ptr - 1)->mode = (*(mergedPtr))->mode;
        } else {
            memcpy(ptr, *mergedPtr, sizeof(StatData));
            ptr++;
        }
        mergedPtr++;
    }
    *result_size = ptr - mergedDump;
    free(mergedPtrDump);
    return mergedDump;
}


int comp_cost(const void *a, const void *b) {
    if (((StatData*)a)->cost > ((StatData*)b)->cost) return 1;
    else if(((StatData*)a)->cost < ((StatData*)b)->cost) return -1;
    else return 0;
}

void SortDump(StatData *data, size_t size){
    qsort(data, size, sizeof(StatData), comp_cost);
}