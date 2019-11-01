#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#define N (int)1e3

void multiplicar(int**, int**, int**);
void gerarMatriz(int**, int**, int**);
void imprimir(int**);
void salvarArquivo(char*, int**);

int main(){
    double inicio, fim;
    int **matriz1;
    int **matriz2;
    int **resultado;
    matriz1 = (int**)malloc(sizeof(int*) * N);
    matriz2 = (int**)malloc(sizeof(int*) * N);
    resultado = (int**)malloc(sizeof(int*) * N);
    for (int i = 0; i < N; i++)
    {
        matriz1[i] = (int*)malloc(sizeof(int) * N);
        matriz2[i] = (int*)malloc(sizeof(int) * N);
        resultado[i] = (int*)malloc(sizeof(int) * N);
    }
    
    gerarMatriz(matriz1, matriz2, resultado);
    inicio = omp_get_wtime();
    multiplicar(matriz1, matriz2, resultado);
    fim = omp_get_wtime();
    printf("Tempo: %lf\n", fim-inicio);
    // imprimir(resultado);
    salvarArquivo("matrizCPararela.txt", resultado);
    for (int i = 0; i < N; i++)
    {
        free(matriz1[i]);
        free(matriz2[i]);
        free(resultado[i]);
    }
    free(matriz1);
    free(matriz2);
    free(resultado);
    return 0;
}

void gerarMatriz(int **mat1, int **mat2, int **resultado){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            mat1[i][j] = 1;
            mat2[i][j] = 1;
            resultado[i][j] = 0; 
        }
    }
}

void multiplicar(int **mat1, int **mat2, int **resultado) { 
    for(int i = 0; i < N; i++) { 
        for(int j = 0; j < N; j++){ 
            for(int k = 0; k < N; k++) 
                resultado[i][j] += (mat1[i][k])*(mat2[k][j]); 
        } 
    } 
} 

void imprimir(int **res){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            printf("%d \t", res[i][j]);
        printf("\n");
    }
}

void salvarArquivo(char *nome, int **m){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w"); //abertura/criação do arquivo 
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            fprintf(pont_arq, "%d\t", m[i][j]); //escrita no arquivo
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq); //fechamento do arquivo
}
