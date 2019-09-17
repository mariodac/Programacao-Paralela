#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "omp.h"
#define TAM 10000
#define N_THREADS 4
#define N_ARGS 4

long int mat[TAM][TAM];

typedef struct {
    int inicio;
    int fim;
}ARG;

void *operacaoMatriz(void*);
void salvarArquivo(char*);

int main(){
    double start, end;

    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            mat[i][j] = 0;
    /* threads e argumentos sem uso de vetores */
    // pthread_t t1, t2;
    // struct args_struct arg1, arg2;
    // arg1.inicio = 0;
    // arg1.fim = TAM / N_ARGS;
    // arg2.inicio = arg1.fim;
    // arg2.fim = arg2.inicio + (TAM / N_ARGS);
    // pthread_create(&t1, NULL, operacaoMatriz, (void*)&arg1);
    // pthread_create(&t2, NULL, operacaoMatriz, (void*)&arg2);
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    /* threads e argumentos sem uso de vetores */

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
            pthread_create(&threads[i], NULL, operacaoMatriz, &args[c]);
            c++;
            // pthread_join(threads[i], NULL);
        }
    }
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL);
    end = omp_get_wtime();
    printf("Tempo: %lf segundos\n", end - start);
    /* usando vetor para threads e argumentos */
    
    salvarArquivo("matrizThread.txt");

    pthread_exit(NULL);
}

void *operacaoMatriz(void *arguments){
    ARG *args = arguments;
    printf("Inicio: %d Fim: %d\n", args->inicio, args->fim);
    for(int i = args->inicio; i < args->fim; i++){
        for (int j = 0; j < TAM; j++)
        {
            mat[i][j] = (i + j) - (i * j);
        }
    }
    pthread_exit(NULL);
}

void salvarArquivo(char *nome){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%ld\t", mat[i][j]);
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);
}