#include "stdio.h"
#include "stdlib.h"
#define N 3

void multiplicar(int[][N], int[][N], int[][N]);
void gerarMatriz(int[][N], int[][N]);
void imprimir(int[][N]);

int main(){
    int matriz1[N][N];
    int matriz2[N][N];
    int resultado[N][N];
    gerarMatriz(matriz1, matriz2);
    multiplicar(matriz1, matriz2, resultado);
    imprimir(resultado);
    return 0;
}

void gerarMatriz(int mat1[][N], int mat2[][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            mat1[i][j] = 1;
            mat2[i][j] = 2;
        }
    }
}

void multiplicar(int mat1[][N], int mat2[][N], int res[][N]) { 
    for(int i = 0; i < N; i++) { 
        for(int j = 0; j < N; j++){ 
            res[i][j] = 0; 
            for(int k = 0; k < N; k++) 
                res[i][j] += mat1[i][k]*mat2[k][j]; 
        } 
    } 
} 

void imprimir(int res[][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            printf("%d \t", res[i][j]);
        printf("\n");
    }
}