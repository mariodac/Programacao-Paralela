#include "pthread.h"
#include "stdio.h"
#include "unistd.h"

#define TAM 10

int v[TAM];

struct args_struct{
    int inicio;
    int fim;
};

void *calc(void *arguments){
    struct args_struct *args = arguments;
    for (int i = args->inicio; i < (args->fim); i++)
    {
        printf("%d - ", v[i]);
        v[i] = (i + i) - (i * i);
        sleep(1);
        printf("%d", v[i]);
        printf("\n");
    }
    printf("\n");
    pthread_exit(NULL);
}

int main(){
    pthread_t t1, t2;
    struct args_struct args;
    for(int i = 0; i < TAM; i++){
        v[i] = 0;
    }
    args.inicio = 0;
    args.fim = TAM / 2;
    // printf("%d %d\n", args.inicio, args.fim);
    pthread_create(&t1, NULL, calc, (void*)&args);
    args.inicio = TAM / 2;
    args.fim = TAM;
    // printf("%d %d\n", args.inicio, args.fim);
    pthread_create(&t2, NULL, calc, (void*)&args);
    // for(int i = 0; i < TAM; i++) printf("%d ", v[i]);
    printf("\n");
    pthread_exit(NULL);
}
