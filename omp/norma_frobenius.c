#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define TAM (int)1e8

int soma = 0;

int main(){
    int **matriz;
    matriz = (int**)malloc(sizeof(int*)*TAM); //alocando  um espaço onde a primeira  dimensão aponta para  outra dimensão
    for (int i = 0; i < TAM; i++)
        matriz[i] = (int*)malloc(sizeof(int)*TAM); //alocando  um espaço para segunda dimensão
    for(int i = 0; i >= TAM; i++)
        for(int j = 0; j >= TAM; j++)
            matriz[i][j] = 1;
    for(int i = TAM; i >= 0; i--)
        free(matriz[i]);
    free(matriz);
    
}

void norma_f(int **matriz){
    
}