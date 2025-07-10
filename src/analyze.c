#include <stdio.h>
#include <stdlib.h>
#include "analyze.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

void PrintDump(const StatData *data, size_t size, size_t count){
    
    if (count > size) count = size;

    for (size_t i = 0; i < count; i++)
        printf("id = %08lX | count = %5d | cost = %.3e| primary = %s | mode = %c%c%c\n",
             (data+i)->id, (data+i)->count, (data+i)->cost, (data+i)->primary == 0 ? "n" : "y",
             (data+i)->mode & 4 ? '1': '0', (data+i)->mode & 2 ? '1': '0', (data+i)->mode & 1 ? '1': '0');
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

// StatData *JoinDump(const StatData *data1, int size1, const StatData *data2, int size2, int *result_size ){

// }
// StatData *SortDump(const StatData *data, int size){

// }