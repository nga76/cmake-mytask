#include <stdio.h>
#include <stdlib.h>
#include "analyze.h"
const StatData case_1_in_a[2] =
{{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};

int main(int argc, char** argv) {
    
    PrintDump(case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData), sizeof(case_1_in_a)/sizeof(StatData));
    
    
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
