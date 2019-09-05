#include "pthread.h"
#include "stdio.h"
#include "math.h"

struct args_struct{
    int tamanho;
    int vetor[10];
};

void *metade1(void *arguments){
    struct args_struct *args = arguments;
    printf("metade1\n");
    for (int i = 0; i < (args->tamanho/2); i++)
    {
        args->vetor[i] = i * pow(-1, i);
        printf("Metade1: %d\n", i);
    }
    pthread_exit(NULL);
}



void *metade2(void *arguments){
    struct args_struct *args = arguments;
    printf("metade2\n");
    for (int i = (args->tamanho/2); i < args->tamanho; i++)
    {
        args->vetor[i] += i * pow(-1, i);
        printf("Metade2: %d\n", i);
    }
    pthread_exit(NULL);
}

void *imprime(void *arguments){
    struct args_struct *args = arguments;
    for (int i = 0; i < args->tamanho; i++)
    {
        printf("Elemento %d: %d\n", i, args->vetor[i]);
    }
    pthread_exit(NULL);
}
int main(){
    pthread_t t1, t2, t3;
    struct args_struct args;
    args.tamanho = 15;
    pthread_create(&t1, NULL, metade1, (void*)&args);
    pthread_create(&t2, NULL, metade2, (void*)&args);
    pthread_create(&t3, NULL, imprime, (void*)&args);
    pthread_exit(NULL);
}
