#include <stdio.h>
#include <stdlib.h>
#include "analyze.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

void PrintDump(StatData *data, int size, int count){

}

int StoreDump(const char *filename, const StatData *data, int size){
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("error:");
        return -1;
    }

    // Записываем размер массива в начало файла
    fwrite(&size, sizeof(int), 1, file);
    
    // Записываем каждую структуру в файл
    for (int i = 0; i < size; i++) {
        fwrite(&data[i], sizeof(StatData), 1, file);
    }

    fclose(file);
    return 0;
}
}
StatData *LoadDump(const char *filename, int *size){

}

StatData *JoinDump(const StatData *data1, int size1, const StatData *data2, int size2, int *result_size ){

}
StatData *SortDump(const StatData *data, int size){

}