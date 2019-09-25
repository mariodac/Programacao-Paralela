#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "omp.h"
#define TAM 30000
#define N_THREADS 4
#define N_ARGS 4

long int matriz[TAM][TAM];
long int matriz_soma[TAM][TAM];

typedef struct {
    int inicio;
    int fim;
}ARG;

void *operacaoMatriz(void*);
void *matrizSoma(void*);
void salvarArquivo(char*, long int[][TAM]);

int main(){
    double start, end;

    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++){
            matriz[i][j] = 0;
            matriz_soma[i][j] = 0;
        }
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
    // start = omp_get_wtime();
    // salvarArquivo("matrizThread.txt");
    // end = omp_get_wtime();
    // printf("Tempo salvando o arquivo: %lf\n", end - start);
    pthread_exit(NULL);
}

void *operacaoMatriz(void *arguments){
    ARG *args = arguments;
    int *valor;
    int i = args->inicio;
    // printf("Inicio: %d Fim: %d\n", args->inicio, args->fim);
    for(; i < args->fim; i++){
        for (int j = 0; j < TAM; j++)
        {
            matriz[i][j] = (i + j) - (i * j);
        }
    }
    *valor = i;
    // return valor;
    
    pthread_exit(NULL);
}

void *matrizSoma(void *arguments){
    ARG *args = arguments;
    int sup, inf, esq, dir;
    int i = args->inicio;
    for (; i < args->fim; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            if(i - 1 < 0 && j - 1 < 0){
                dir = j + 1;
                inf = i + 1;
                esq = 0;
                sup = 0;
            }
            else if(i + 1 == TAM && j + 1 == TAM){
                esq = j - 1;
                sup = i - 1;
                dir = TAM - 1;
                inf = TAM - 1;
            }
            else if(i + 1 == TAM && j - 1 < 0){
                sup = i - 1;
                dir = j + 1;
                inf = TAM - 1;
                esq = 0;
            }
            else if(i - 1 < 0 && j + 1 == TAM){
                inf = i + 1;
                esq = j - 1;
                sup = 0;
                dir = TAM - 1;
            }
            else if(j - 1 < 0){
                sup = i - 1;
                inf = i + 1;
                esq = 0;
                dir = j + 1;
            }
            else if(i - 1 < 0){
                sup = 0;
                inf = i + 1;
                esq = j - 1;
                dir = j + 1; 
            }
            else if(i + 1 == TAM){
                sup = i - 1;
                inf = TAM - 1;
                esq = j - 1;
                dir = j + 1;
            }
            else if(j + 1 == TAM){
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = TAM - 1;
            }
            else{
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = j + 1;
            }
            matriz_soma[i][j] = matriz[inf][j] + matriz[sup][j] + matriz[i][esq] + matriz[i][dir];
        }
    }
}

void salvarArquivo(char *nome, long int m[][TAM]){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%ld\t", m[i][j]);
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);
}