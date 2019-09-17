#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define TAM 10000

int v[TAM];

void operacaoVetor(int*, int);

int main(){
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE *pont_arq;
    pont_arq = fopen("vetorSerial.txt", "w");

    for(int i = 0; i < TAM; i++){
        v[i] = 0;
    }
    operacaoVetor(v, TAM);
    // for(int i = 0; i < tamanho; i++) printf("%d ", vetor[i]);
    // printf("\n");

    for (int i = 0; i < TAM; i++)
    {
        fprintf(pont_arq, "%d\n", v[i]);
    }
    fclose(pont_arq);

    end = clock();
    cpu_time_used = ((double) (end - start)) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo: %.2f ms\n", cpu_time_used);
    
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