#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM (int)1e4

void multiplicarDuasMatrizes(int**, int**, int**);
void imprimir(int**);
int main()
{
    int **matrizA, **matrizB, **matrizC;
    double inicio, fim;
    matrizA = (int **)malloc(sizeof(int *) * TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    matrizB = (int **)malloc(sizeof(int *) * TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    matrizC = (int **)malloc(sizeof(int *) * TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    for (int i = 0; i < TAM; i++)
    {
        matrizA[i] = (int *)malloc(sizeof(int) * TAM); //alocando  um espaço para segunda dimensão
        matrizB[i] = (int *)malloc(sizeof(int) * TAM); //alocando  um espaço para segunda dimensão
        matrizC[i] = (int *)malloc(sizeof(int) * TAM); //alocando  um espaço para segunda dimensão
    }
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            matrizA[i][j] = 1;
            matrizB[i][j] = 1;
            matrizC[i][j] = 0;
        }
    }
    inicio = omp_get_wtime();
    multiplicarDuasMatrizes(matrizA, matrizB, matrizC);
    fim = omp_get_wtime();
    printf("Tempo: %lf\n", fim-inicio);
    // imprimir(matrizC);
    for(int i = TAM; i >= 0; i--){
        free(matrizA[i]);
        free(matrizB[i]);
        free(matrizC[i]);
    }
    free(matrizA);
    free(matrizB);
    free(matrizC);
    return 0;
}

void multiplicarDuasMatrizes(int **matrizA, int **matrizB, int **matrizC){
    int i, j, k;
    // #pragma omp parallel for default(none) private(i, j, k) shared(matrizA, matrizB, matrizC) collapse(3) schedule(static) num_threads(2)
    for (i = 0; i < TAM; i++)
    {
        for (j = 0; j < TAM; j++)
        {
            for (k = 0; k < TAM; k++)
            {
                // if(k == 0)
                    // printf("Total threads: %d\n", omp_get_num_threads());
                matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
            }
            
        }
        
    }
    
}

void imprimir(int **matriz){
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++)
            printf("%d \t", matriz[i][j]);
        printf("\n");
    }
}