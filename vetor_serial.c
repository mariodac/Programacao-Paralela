#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define TAM 10000000000

int v[TAM];

void operacaoVetor(int*, int);
void salvarArquivo(char*);

int main(){
    double start, end;

    for(int i = 0; i < TAM; i++)
        v[i] = 0;
    start = omp_get_wtime();
    operacaoVetor(v, TAM);
    end = omp_get_wtime();
    printf("Tempo: %lf segundos\n", end - start);
    salvarArquivo("vetorSerial.txt");
    return 0;
}

void operacaoVetor(int *v, int t){
    for(int i = 0; i < t; i++){
        // printf("%d -> ", i);
        v[i] = (i + i) - (i * i);
        // printf("%d", v[i]);
        // printf("\n");
    }
}

void salvarArquivo(char *nome){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        fprintf(pont_arq, "%d\n", v[i]);
    }
    fclose(pont_arq);
}