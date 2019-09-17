#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define TAM 10000
long long int matriz[TAM][TAM];

void operacaoMatriz(long long int[][TAM], int);

int main(){
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE *pont_arq;
    pont_arq = fopen("matrizSerial.txt", "w");

    int tamanho = TAM;
    for(int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            matriz[i][j] = 0;
    operacaoMatriz(matriz, tamanho);
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = 0; j < tamanho; j++)
            fprintf(pont_arq, "%lld\t", matriz[i][j]);
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);

    end = clock();
    cpu_time_used = ((double) (end - start)) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo: %.2f ms\n", cpu_time_used);
    return 0;
}

void operacaoMatriz(long long int m[][TAM], int t){
    for(int i = 0; i < t; i++){
        for (int j = 0; j < t; j++)
        {
            m[i][j] = (i + j) - (i * j);
        }
    }
}