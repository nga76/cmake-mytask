#include <stdio.h>
#include "analyze.h"

int main(int argc, char** argv) {
    if (argc < 4)
    {
        fprintf(stderr, "Not enough parameters\n");
        exit(1);
    }
    printf("long is %lu byte\n", sizeof(long));
    printf("Hello, from analyze!\n");
    for (int i=0;i<argc;i++)
    {
        printf("param %d = %s\n", i, argv[i]);
    }

    
    
    int a = 10;
    int b = 5;
    
    printf("Сложение: %d + %d = %d\n", a, b, add(a, b));
    printf("Вычитание: %d - %d = %d\n", a, b, subtract(a, b));
    
    return 0;
}
