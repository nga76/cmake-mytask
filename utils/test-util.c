#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "analyze.h"

#define BUF_SZ (1024)

/* Содержимое для исходных файлов */
const StatData case_1_in_a[2] =
{{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode=3 },
{.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode=0 }};

const StatData case_1_in_b[2] =
{{.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode=2 },
{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode=2}};

/* Ожидаемый результат обработки */
const StatData case_1_out[3] =
{{.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2 },
{.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3 },
{.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2 }};

int main(int argc, char** argv) {
    clock_t start = clock();
    int err = 0;
    FILE *fp;
    char cwd[BUF_SZ]={'\0'};
    char in1[2*BUF_SZ]={'\0'};
    char in2[2*BUF_SZ]={'\0'};
    char out[2*BUF_SZ]={'\0'};
    size_t out_sz = 0;  
    StatData *out_data = NULL;
    if (argc != 2) {
        perror("error: you need to send path to util as param");
        err = -1;
        goto free_res;
    }
    
    if(getcwd(cwd,BUF_SZ) == NULL){
        perror("error: can't identify the working directory");
        err = -2;
        goto free_res;
    }
    
    sprintf(out, "%s/%s", cwd, "case_1_out");

    sprintf(in1, "%s/%s", cwd, "case_1_in_a");
    if (StoreDump("case_1_in_a", case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData))){
        perror("error: can't save output data case_1_in_a");
        err = -3;
        goto free_res;
    }

    sprintf(in2, "%s/%s", cwd, "case_1_in_b");
    if (StoreDump("case_1_in_b", case_1_in_b, sizeof(case_1_in_b)/sizeof(StatData))){
        perror("error: can't save output data case_1_in_b");
        err = -4;
        goto free_res;
    }
    char cmd[8*BUF_SZ];
    sprintf(cmd, "%s 2>&1 %s %s %s", argv[1], in1, in2, out);
    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("error: Failed to run command");
        printf("Failed to run command\n" );
        err = -5;
        goto free_res;
    }
    // char rbuf[BUF_SZ];
    // while (fgets(rbuf, sizeof(rbuf), fp) != NULL) {
    //     printf("%s", rbuf);
    // }
    pclose(fp);
    
    out_data = LoadDump(out, &out_sz);
    if (NULL == out_data){
        perror("error: can't read output file");
        err = -6;
        goto free_res;
    }
    if (memcmp(case_1_out, out_data, out_sz*sizeof(StatData)) == 0){
        clock_t end = clock();
        printf("Test passed : %f s\n", (float)(end - start) / CLOCKS_PER_SEC);
    }else{
        perror("Test filed");
        err = -7;
    }


    // free memory
    free_res: {
        // free(output);
        free(out_data);
    }
    if ( err ) return err; 
    return EXIT_SUCCESS;
}
