#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "analyze.h"

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


// Функция для запуска программы и получения вывода
char* run_program(const char* command, const char** args, size_t* output_size) {
    int pipefd[2];
    pid_t child_pid;
    char* output = NULL;
    size_t buf_size = 0;
    ssize_t bytes_read;
    
    // Создаем pipe для захвата вывода
    if (pipe(pipefd) == -1) {
        perror("Ошибка создания pipe");
        return NULL;
    }
    
    // Создаем дочерний процесс
    child_pid = fork();
    if (child_pid == -1) {
        perror("Ошибка создания процесса");
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }
    
    if (child_pid == 0) {
        // В дочернем процессе
        close(pipefd[0]); // Закрываем чтение из pipe
        
        // Перенаправляем stdout в write end pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Ошибка перенаправления stdout");
            _exit(1);
        }
        close(pipefd[1]); // Закрываем оригинальный файловый дескриптор
        
        // Запускаем программу
        execvp(command, (char* const*)args);
        perror("Ошибка выполнения программы");
        _exit(1);
    } else {
        // В родительском процессе
        close(pipefd[1]); // Закрываем write end pipe
        
        // Читаем вывод из pipe
        char buffer[4096];
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            
            // Увеличиваем буфер
            buf_size += bytes_read;
            output = realloc(output, buf_size + 1);
            if (output == NULL) {
                perror("Ошибка выделения памяти");
                close(pipefd[0]);
                return NULL;
            }
            strcat(output, buffer);
        }
        close(pipefd[0]);
        
        // Ждем завершения дочернего процесса
        wait(NULL);
        
        if (output_size)
            *output_size = buf_size;
        
        return output;
    }
}

// Пример использования
int main(int argc, char** argv) {
    if (argc == 2) {
        printf("%s\n", argv[1]);
    }
    
    char *cwd = getcwd(NULL,0);
    printf("%s\n", cwd);

    if (StoreDump("case_1_in_a", case_1_in_a, sizeof(case_1_in_a)/sizeof(StatData))){
        perror("Can't save output data case_1_in_a");
    }

    if (StoreDump("case_1_in_b", case_1_in_a, sizeof(case_1_in_b)/sizeof(StatData))){
        perror("Can't save output data case_1_in_b");
    }

    

    const char* command = "/home/gne/CFT/cmake-mytask/build/analyze-util";
    const char* args[] = {command, NULL};
    size_t output_size;
    char* output = run_program(command, args, &output_size);
    
    if (output) {
        printf("Вывод программы:\n%s\n", output);
        free(output);
    } else {
        printf("Не удалось получить вывод программы\n");
    }
    
    return 0;
}
