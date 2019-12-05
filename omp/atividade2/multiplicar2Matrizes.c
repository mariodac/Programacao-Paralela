#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM (int)1e3
#define N_THREADS 16

void salvarArquivo(char*, int**);
void multiplicarMatrizesSerial(int**, int**, int**);
void multiplicarMatrizesParalelo(int**, int**, int**);
void gerarMatriz(int**, int**, int**);

int main()
{
    double inicioSerial = 0, fimSerial = 0, inicioParelelo = 0, fimParelelo = 0, speedup = 0, eficiencia = 0, tempoSerial = 0, tempoParelelo = 0;
    int **matrizA, **matrizB, **matrizC, numero_threads;
    matrizA = (int**)malloc(sizeof(int*) * TAM);
    matrizB = (int**)malloc(sizeof(int*) * TAM);
    matrizC = (int**)malloc(sizeof(int*) * TAM);
    for (int i = 0; i < TAM; i++)
    {
        matrizA[i] = (int*)malloc(sizeof(int) * TAM);
        matrizB[i] = (int*)malloc(sizeof(int) * TAM);
        matrizC[i] = (int*)malloc(sizeof(int) * TAM);
    }
    gerarMatriz(matrizA, matrizB, matrizC);
    inicioSerial = omp_get_wtime();
    multiplicarMatrizesSerial(matrizA, matrizB, matrizC);
    fimSerial = omp_get_wtime();
    tempoSerial = fimSerial - inicioSerial;
    printf("Serial: %.4f\n", tempoSerial);
    salvarArquivo("matrizA.txt", matrizA);
    salvarArquivo("matrizB.txt", matrizB);
    salvarArquivo("matrizCSerial.txt", matrizC);
    gerarMatriz(matrizA, matrizB, matrizC);
    inicioParelelo = omp_get_wtime();
    multiplicarMatrizesParalelo(matrizA, matrizB, matrizC);
    fimParelelo = omp_get_wtime();
    tempoParelelo = fimParelelo - inicioParelelo;
    printf("Paralelo: %.4lf\n", tempoParelelo);
    speedup = tempoSerial / tempoParelelo;
    eficiencia = speedup / N_THREADS;
    printf("SPEEDUP: %.4f\n", speedup);
    printf("EFICIENCIA: %.4f\n", eficiencia);
    salvarArquivo("matrizCParalelo.txt", matrizC);

    for(int i = TAM; i >= 0; i--)
    {
        free(matrizA[i]);
        free(matrizB[i]);
        free(matrizC[i]);
    }
    free(matrizA);
    free(matrizB);
    free(matrizC);
    return 0;
}

void salvarArquivo(char *nome, int **m){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w"); //abertura/criação do arquivo 
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%d\t", m[i][j]); //escrita no arquivo
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq); //fechamento do arquivo
}

void gerarMatriz(int **matrizA, int **matrizB, int **matrizC){
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            matrizA[i][j] = 1;
            matrizB[i][j] = 1;
            matrizC[i][j] = 0;
        }
    }
}
void multiplicarMatrizesSerial(int **matrizA, int **matrizB, int **matrizC)
{
    for(int i = 0; i < TAM; i++) { 
        for(int j = 0; j < TAM; j++){ 
            for(int k = 0; k < TAM; k++) 
                matrizC[i][j] += (matrizA[i][k])*(matrizB[k][j]); 
        } 
    } 
}
void multiplicarMatrizesParalelo(int **matrizA, int **matrizB, int **matrizC)
{
    int i, j, k, numero_threads = 0;
    #pragma omp parallel for collapse(3) default(none) private(i, j, k) shared(matrizA, matrizB, matrizC) num_threads(N_THREADS)
    for (i = 0; i < TAM; i++)
    {
        for (j = 0; j < TAM; j++)
        {
            for (k = 0; k < TAM; k++)
            {
                matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
            }
            
        }
        
    }
}