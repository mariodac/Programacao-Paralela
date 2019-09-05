#include "stdio.h"

int fibo(int);
void imprimir(int);

int main()
{
    int numero;
    printf("Digite um numero: ");
    scanf("%d", &numero);
    imprimir(numero);
    return 0;
}

int fibo(int num){
    if (num == 0)
        return 0;
    else if (num == 1)
        return 1;
    else
        return(fibo(num - 1) + fibo(num - 2));
}

void imprimir(int num){
    printf("Sequência Fibonacci de %d = ", num);
    for(int i = 0; i <= num; i++)
        printf("%d ", fibo(i));
    printf("\n");
}
