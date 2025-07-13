#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyze.h"
const StatData case_1_in_a_[2] =
{{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};

const StatData case_1_in_b_[2] =
{{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};

int main(int argc, char** argv) {
    StatData *case_1_in_a = NULL;
    StatData *case_1_in_b = NULL;
    StatData *case_1_out = NULL;
    size_t size_case_1_in_a = 0;
    size_t size_case_1_in_b = 0;
    size_t size_case_1_out = 0;

    
    // StoreDump
    
    // check params
    // StoreDump("case_1_in_a", case_1_in_a_, sizeof(case_1_in_a_)/sizeof(StatData));
    // StoreDump("case_1_in_b", case_1_in_b_, sizeof(case_1_in_b_)/sizeof(StatData));
    if (argc < 4) {
        const char* last_slash = strrchr(argv[0], '/');
        if (argc == 2 && ( strcasecmp(argv[1], "-h") == 0 || strcasecmp(argv[1], "--help") == 0)) {
            printf("Run:\n\t%s path_in1 path_in2 path_out\n", last_slash+1);
        }else{
            perror("Wrong parameters\n");
            printf("Run:\n\t%s path_in1 path_in2 path_out\n", last_slash+1);
        }
        exit(1);
    }
    
    // load data
    case_1_in_a = LoadDump(argv[1], &size_case_1_in_a);
    PrintDump(case_1_in_a, size_case_1_in_a, 10);
    case_1_in_b = LoadDump(argv[2], &size_case_1_in_b);
    PrintDump(case_1_in_b, size_case_1_in_b, 10);

    // join data
    case_1_out = JoinDump(case_1_in_a, size_case_1_in_a,
                          case_1_in_b, size_case_1_in_b,
                          &size_case_1_out);

    // sort rezult
    SortDump(case_1_out, size_case_1_out);

    // print rezult
    PrintDump(case_1_out, size_case_1_out, 10);

    // store rezult
    if (StoreDump(argv[3], case_1_out, size_case_1_out)){

    }
    
    // free memory
    free(case_1_in_a);
    free(case_1_in_b);
    free(case_1_out);
    return 0;
}
