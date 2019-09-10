#include "stdio.h"

void operacaoVetor(int*, int);

int main(){
    int tamanho = 10;
    int vetor[tamanho];
    for(int i = 0; i < tamanho; i++){
        vetor[i] = 0;
    }
    operacaoVetor(vetor, tamanho);
    for(int i = 0; i < tamanho; i++) printf("%d ", vetor[i]);
    printf("\n");
    return 0;
}

void operacaoVetor(int *v, int t){
    for(int i = 0; i < t; i++){
        printf("%d -> ", v[i]);
        v[i] = (i + i) - (i * i);
        printf("%d", v[i]);
        printf("\n");
    }
}