#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyze.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

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
        perror("Ошибка при открытии файла для записи");
        return -1;
    }

    // Записываем размер массива в начало файла
    fwrite(&size, sizeof(size_t), 1, file);
    
    // Записываем каждую структуру в файл
    for (size_t i = 0; i < size; i++) {
        fwrite(&data[i], sizeof(StatData), 1, file);
    }

    fclose(file);
    return 0;
}

StatData *LoadDump(const char *filename, size_t *size){
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла для чтения");
        return NULL;
    }

    // Читаем размер массива
    if (fread(size, sizeof(size_t), 1, file) != 1) {
        perror("Ошибка при чтении размера массива");
        fclose(file);
        return NULL;
    }

    // Выделяем память под массив
    StatData *data = (StatData *)malloc(*size * sizeof(StatData));
    if (data == NULL) {
        perror("Ошибка при выделении памяти");
        fclose(file);
        return NULL;
    }

    // Читаем структуры из файла
    if (fread(data, sizeof(StatData), *size, file) != *size) {
        perror("Ошибка при чтении данных");
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
        perror("Ошибка выделения памяти");
        return NULL;
    }
    if (!mergedPtrDump) {
        perror("Ошибка выделения памяти");
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