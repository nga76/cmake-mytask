#include <stdio.h>
#include "analyze.h"

int main() {
    int a = 10;
    int b = 5;
    
    printf("Сложение: %d + %d = %d\n", a, b, add(a, b));
    printf("Вычитание: %d - %d = %d\n", a, b, subtract(a, b));
    
    return 0;
}
