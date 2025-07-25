#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "analyze.h"


int main(int argc, char** argv) {
    int err = 0;
    StatData *case_1_in_a = NULL;
    StatData *case_1_in_b = NULL;
    StatData *case_1_out = NULL;
    size_t size_case_1_in_a = 0;
    size_t size_case_1_in_b = 0;
    size_t size_case_1_out = 0;
    
    // check params
    if (argc < 4) {
        const char* last_slash = strrchr(argv[0], '/');
        if (argc == 2 && ( strcasecmp(argv[1], "-h") == 0 || strcasecmp(argv[1], "--help") == 0)) {
            printf("Run:\n\t%s path_in1 path_in2 path_out\n", last_slash+1);
        }else{
            perror("Wrong parameters");
            printf("Run:\n\t%s path_in1 path_in2 path_out\n", last_slash+1);
        }
        err = -1;
        goto free_res;
    }
    
    // load data
    case_1_in_a = LoadDump(argv[1], &size_case_1_in_a);
    if (case_1_in_a == NULL){
        perror("Can't load input data 1");
        err = errno;
        goto free_res;
    }
    
    case_1_in_b = LoadDump(argv[2], &size_case_1_in_b);
    if (case_1_in_b == NULL){
        perror("Can't load input data 2");
        err = errno;
        goto free_res;
    }
    

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
        perror("Can't save output data");
        goto free_res;
    }
    
    // free memory
    free_res: {
        free(case_1_in_a);
        free(case_1_in_b);
        free(case_1_out);
    }
    if ( err ) return err; 
    return EXIT_SUCCESS;
}
