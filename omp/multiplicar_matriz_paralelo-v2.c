#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM (int)2e3

int multiplicarDuasMatrizes(int**, int**, int**);
void gerarMatriz(int**, int**, int**);
void imprimir(int**);
void salvarArquivo(char*, int**);

int main()
{
    double inicio, fim;
    int **matrizA, **matrizB, **matrizC, numero_threads;
    matrizA = (int**)malloc(sizeof(int*) * TAM);
    matrizB = (int**)malloc(sizeof(int*) * TAM);
    matrizC = (int**)malloc(sizeof(int*) * TAM);
    for (int i = 0; i < TAM; i++)
    {
        matrizA[i] = (int*)malloc(sizeof(int) * TAM);
        matrizB[i] = (int*)malloc(sizeof(int) * TAM);
        matrizC[i] = (int*)malloc(sizeof(int) * TAM);
    }
    gerarMatriz(matrizA, matrizB, matrizC);
    inicio = omp_get_wtime();
    numero_threads = multiplicarDuasMatrizes(matrizA, matrizB, matrizC);
    fim = omp_get_wtime();
    printf("Tempo: %lf\n", fim-inicio);
    // printf("Threads: %d\n", numero_threads);
    // printf("Tamanho: %d\n", TAM);
    // imprimir(matrizC);
    char nome[30];
    sprintf(nome, "matrizCParelela2%d.txt", TAM);
    salvarArquivo(nome, matrizC);
    for(int i = TAM; i >= 0; i--)
    {
        free(matrizA[i]);
        free(matrizB[i]);
        free(matrizC[i]);
    }
    free(matrizA);
    free(matrizB);
    free(matrizC);
    return 0;
}

int multiplicarDuasMatrizes(int **matrizA, int **matrizB, int **matrizC){
    int i, j, k, numero_threads = 0;
    #pragma omp parallel  default(none) private(i, j, k) shared(numero_threads, matrizA, matrizB, matrizC)  num_threads(2)
    {
        if(omp_get_thread_num() == 0)
            numero_threads = omp_get_num_threads();
        #pragma omp for collapse(2)
        for (i = 0; i < TAM; i++)
        {
            for (j = 0; j < TAM; j++)
            {
                for (k = 0; k < TAM; k++)
                {
                    matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
                }
                
            }
            
        }
    }
    return numero_threads;
    
}

void imprimir(int **matriz){
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++)
            printf("%d \t", matriz[i][j]);
        printf("\n");
    }
}

void salvarArquivo(char *nome, int **m){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w"); //abertura/criação do arquivo 
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%d\t", m[i][j]); //escrita no arquivo
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq); //fechamento do arquivo
}
void gerarMatriz(int **matrizA, int **matrizB, int **matrizC){
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            matrizA[i][j] = 1;
            matrizB[i][j] = 1;
            matrizC[i][j] = 0;
        }
    }
}