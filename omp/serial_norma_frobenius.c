#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define TAM (int)3.6e4
int soma = 0;
int norma_f(int**);

int main(){
    int **matriz;
    double inicio, fim;
    matriz = (int**)malloc(sizeof(int*)*TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    for (int i = 0; i < TAM; i++)
        matriz[i] = (int*)malloc(sizeof(int)*TAM); //alocando  um espaço para segunda dimensão
    for(int i = 0; i < TAM; i++)
        for(int j = 0; j < TAM; j++)
            matriz[i][j] = 1;
    inicio = omp_get_wtime();
    printf("Norma: %d\n", norma_f(matriz));
    fim = omp_get_wtime();
    printf("Tempo: %lf\n", fim-inicio);
    for(int i = TAM; i >= 0; i--)
        free(matriz[i]);
    free(matriz);
    
}

int norma_f(int **matriz){
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            soma += abs(pow(matriz[i][j], 2));
        
    return soma;
}