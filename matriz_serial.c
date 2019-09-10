#include "stdio.h"
#define TAM 10
void operacaoMatriz(int[][TAM], int);

int main(){
    int tamanho = TAM;
    int matriz[tamanho][tamanho];
    for(int i = 0; i < tamanho; i++){
        for (int j = 0; j < tamanho; j++)
        {
            matriz[i][j] = 0;
        }
        
    }
    operacaoMatriz(matriz, tamanho);
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
        {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
        
    }
    
    printf("\n");
    return 0;
}

void operacaoMatriz(int m[][TAM], int t){
    for(int i = 0; i < t; i++){
        for (int j = 0; j < t; j++)
        {
            m[i][j] = (i + j) - (i * j);
        }
    }
}