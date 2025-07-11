#include <stdio.h>
#include <stdlib.h>
#include "analyze.h"
const StatData case_1_in_a[3] =
{{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 },
{.id = 90589, .count = 1, .cost = 188.90, .primary = 1, .mode=0 }};

const StatData case_1_in_b[2] =
{{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};

int main(int argc, char** argv) {
    StatData *rdata = NULL;
    size_t load_size = 0;
    
    PrintDump(case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData), sizeof(case_1_in_a)/sizeof(StatData));
    StoreDump("teststor", case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData));
    rdata = LoadDump("teststor", &load_size);
    SortDump(rdata, load_size);
    PrintDump(rdata, load_size, load_size);
    size_t new_size = 0;
    StatData *newDump = JoinDump(case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData),
                                 case_1_in_b, sizeof(case_1_in_b)/sizeof(StatData), &new_size);
    PrintDump(rdata, load_size, load_size);                             
    
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
