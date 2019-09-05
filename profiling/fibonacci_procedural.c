#include "stdio.h"

void fibonacci(int);

void main()
{
    int n;
    printf("Digite um numero: ");
    scanf("%d", &n);
    fibonacci(n);
}


void fibonacci(int n){
    int a, b, auxiliar;
    a = 0;
    b = 1;
    printf("Série de Fibonacci:\n");
    printf("%d\n", a);
    printf("%d\n", b);

    for (int i = 0; i < n-1; i++){
        auxiliar = a + b;
        a = b;
        b = auxiliar;
        printf("%d\n", auxiliar);
    }
}