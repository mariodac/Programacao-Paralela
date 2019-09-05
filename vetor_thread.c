#include "pthread.h"
#include "stdio.h"

struct args_struct{
    int tamanho;
    int vetor[];
};

void *metade1(void *arguments){
    struct args_struct *args = arguments;
    printf("metade1\n");
    for (int i = 0; i < (args->tamanho/2); i++)
    {
        args->vetor[i] = (i + i) - (i * i);
        printf("Metade1: %d\n", i);
    }
    pthread_exit(NULL);
}



void *metade2(void *arguments){
    struct args_struct *args = arguments;
    printf("metade2\n");
    for (int i = (args->tamanho/2); i < args->tamanho; i++)
    {
        args->vetor[i] = (i + i) - (i * i);
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
    for(int i = 0; i < args.tamanho; i++){
        args.vetor[i] = i;
    }
    pthread_create(&t1, NULL, metade1, (void*)&args);
    pthread_create(&t2, NULL, metade2, (void*)&args);
    for(int i = 0; i < args.tamanho; i++) printf("%d ", args.vetor[i]);
    printf("\n");
    pthread_exit(NULL);
}
