#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define TAM 1000000000
#define N_THREADS 2
#define N_ARGS 2

int v[TAM];

typedef struct {
    int inicio;
    int fim;
}ARG;

void *calc(void *arguments){
    ARG *args = arguments;
    // printf("Inicio: %d Fim: %d\n", args->inicio, args->fim);
    for (int i = args->inicio; i < (args->fim); i++)
        v[i] = (i + i) - (i * i);
    pthread_exit(NULL);
}

void salvarArquivo(char *nome){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        fprintf(pont_arq, "%d\n", v[i]);
    }
    fclose(pont_arq);
}

int main(){

    double start, end;
    for(int i = 0; i < TAM; i++)
        v[i] = 0;

    /* usando vetor para threads e argumentos */
    pthread_t threads[N_THREADS];
    ARG args[N_ARGS];
    for (int i = 0; i < N_ARGS; i++)
    {
        if(i == 0){
            args[i].inicio = 0;
            args[i].fim = TAM / N_ARGS;
        }
        else{
            args[i].inicio = args[i-1].fim;
            args[i].fim = args[i].inicio + (TAM/N_ARGS);
        }
    }
    int c = 0;
    start = omp_get_wtime();    
    while (c != N_ARGS)
    {
        for (int i = 0; i < N_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, calc, &args[c]);
            c++;
            // pthread_join(threads[i], NULL);
        }
    }
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);
    /* usando vetor para threads e argumentos */

    /* threads e argumentos sem uso de vetores */
    // pthread_t t1, t2;
    // struct args_struct args1, args2, args3, args4;
    // args1.inicio = 0;
    // args1.fim = TAM / N_ARGS;
    // args2.inicio = args1.fim;
    // args2.fim = args2.inicio + (TAM/N_ARGS);
    // args3.inicio = args2.fim;
    // args3.fim = args3.inicio + (TAM/N_ARGS);
    // args4.inicio = args3.fim;
    // args4.fim = args4.inicio + (TAM/N_ARGS);
    // pthread_create(&t1, NULL, calc, (void*)&args1); //thread t1 args1
    // pthread_create(&t2, NULL, calc, (void*)&args2); //thread t2 args2
    
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);

    // pthread_create(&t1, NULL, calc, &args3);
    // pthread_create(&t2, NULL, calc, &args4);

    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    /* threads e argumentos sem uso de vetores */
    
    end = omp_get_wtime();
    printf("Tempo: %lf segundos\n", end - start);
    salvarArquivo("vetorThread.txt");
    pthread_exit(NULL);
}
