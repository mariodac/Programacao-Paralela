#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define TAM (int)3.6e4

void norma_f(int**);

int main(){
    int **matriz;
    double inicio, fim;
    matriz = (int**)malloc(sizeof(int*)*TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    for (int i = 0; i < TAM; i++)
        matriz[i] = (int*)malloc(sizeof(int)*TAM); //alocando  um espaço para segunda dimensão
    for(int i = 0; i < TAM; i++)
        for(int j = 0; j < TAM; j++)
            matriz[i][j] = 1;
    // for(int i = 0; i < TAM; i++){
    //     for(int j = 0; j < TAM; j++)
    //         printf("%d ", matriz[i][j]);
    //     printf("\n");
    // }
    inicio = omp_get_wtime();
    norma_f(matriz);
    fim = omp_get_wtime();
    printf("Tempo: %lf\n", fim-inicio);
    for(int i = TAM; i >= 0; i--)
        free(matriz[i]);
    free(matriz);
    
}

void norma_f(int **matriz){
    int somaP = 0, i = 0, j = 0;
    int somaT = 0;
    int norma = 0;
    int num_threads = 0;
    #pragma omp parallel default(none) shared(somaT, matriz, num_threads, norma) private(i, j) num_threads(10)
    {
        num_threads = omp_get_num_threads();
        #pragma omp for reduction(+: somaT) collapse(2)
        for (i = 0; i < TAM; i++){
            for (j = 0; j < TAM; j++){
                somaT += pow(matriz[i][j], 2);
            }
        }
        norma = sqrt(somaT);
        // printf("Soma total %d: %d \n",omp_get_thread_num(), somaT);
        // printf("Soma parcial %d: %d\n", omp_get_thread_num(), somaP);
        // #pragma omp atomic
        // somaT += somaP;
        // printf("Soma total %d: %d \n",omp_get_thread_num(), somaT);
        // printf("Soma parcial da thread %d: %d\n", omp_get_thread_num(), somaP);
    }
    
    printf("Totais de threads: %d\n", num_threads);
    printf("Soma: %d\n", somaT);
    printf("Norma: %d\n", norma);
}