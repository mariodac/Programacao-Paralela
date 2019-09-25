#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define TAM 5
long int matriz[TAM][TAM];
long int matriz_soma[TAM][TAM];

void operacaoMatriz(long int[][TAM], int);
void matrizSoma(long int[][TAM], int);
void salvarArquivo(char*, long int[][TAM]);

int main(){
    double start, end;
    int tamanho = TAM;

    for(int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++){
            matriz[i][j] = 0;
            matriz_soma[i][j] = 0;
        }

    start = omp_get_wtime();
    operacaoMatriz(matriz, tamanho);
    matrizSoma(matriz_soma, TAM);
    end = omp_get_wtime();
    printf("Tempo: %lf segundos\n", end - start);
    salvarArquivo("matrizSerial.txt", matriz);
    salvarArquivo("matrizSerialSoma.txt", matriz_soma);

    return 0;
}

void operacaoMatriz(long int m[][TAM], int t){
    for(int i = 0; i < t; i++)
        for (int j = 0; j < t; j++)
            m[i][j] = (i + j) - (i * j);
}

void matrizSoma(long int m_soma[][TAM], int t){
    int sup, inf, esq, dir;
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            if(i - 1 < 0 && j - 1 < 0){
                dir = j + 1;
                inf = i + 1;
                esq = 0;
                sup = 0;
            }
            else if(i + 1 == TAM && j + 1 == TAM){
                esq = j - 1;
                sup = i - 1;
                dir = TAM - 1;
                inf = TAM - 1;
            }
            else if(i + 1 == TAM && j - 1 < 0){
                sup = i - 1;
                dir = j + 1;
                inf = TAM - 1;
                esq = 0;
            }
            else if(i - 1 < 0 && j + 1 == TAM){
                inf = i + 1;
                esq = j - 1;
                sup = 0;
                dir = TAM - 1;
            }
            else if(j - 1 < 0){
                sup = i - 1;
                inf = i + 1;
                esq = 0;
                dir = j + 1;
            }
            else if(i - 1 < 0){
                sup = 0;
                inf = i + 1;
                esq = j - 1;
                dir = j + 1; 
            }
            else if(i + 1 == TAM){
                sup = i - 1;
                inf = TAM - 1;
                esq = j - 1;
                dir = j + 1;
            }
            else if(j + 1 == TAM){
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = TAM - 1;
            }
            else{
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = j + 1;
            }
            matriz_soma[i][j] = matriz[inf][j] + matriz[sup][j] + matriz[i][esq] + matriz[i][dir];
        }
    }
}

void salvarArquivo(char *nome, long int m[][TAM]){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%ld\t", m[i][j]);
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);
}