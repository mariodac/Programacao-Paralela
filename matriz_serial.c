#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define TAM 99999
long int matriz[TAM][TAM];

void operacaoMatriz(long int[][TAM], int);
void salvarArquivo(char*);

int main(){
    double start, end;
    int tamanho = TAM;

    for(int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++)
            matriz[i][j] = 0;

    start = omp_get_wtime();
    operacaoMatriz(matriz, tamanho);
    end = omp_get_wtime();
    printf("Tempo: %lf segundos\n", end - start);
    salvarArquivo("matrizSerial.txt");
    return 0;
}

void operacaoMatriz(long int m[][TAM], int t){
    for(int i = 0; i < t; i++)
        for (int j = 0; j < t; j++)
            m[i][j] = (i + j) - (i * j);
}

void salvarArquivo(char *nome){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%ld\t", matriz[i][j]);
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);
}