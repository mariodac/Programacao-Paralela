#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM_V (int)1e8
#define N_THREADS 16

int produtoInternoSerial(int*, int*);
int produtoInternoParalelo(int*, int*);
void preencheVetores(int*, int*);
void imprimeVetores(int*, int*);

int main()
{
    int *u, *v;
    u = malloc(sizeof(int) * TAM_V);
    v = malloc(sizeof(int) * TAM_V);
    double inicioSerial = 0, fimSerial = 0, inicioParelelo = 0, fimParelelo = 0, speedup = 0, eficiencia = 0, tempoSerial = 0, tempoParelelo = 0;
    preencheVetores(u, v);
    inicioSerial = omp_get_wtime();
    printf("Produto interno Serial= %d\n", produtoInternoSerial(u, v));
    fimSerial = omp_get_wtime();
    tempoSerial = fimSerial - inicioSerial;
    printf("Serial: %.4lf\n", tempoSerial);
    inicioParelelo = omp_get_wtime();
    printf("Produto interno Paralelo= %d\n", produtoInternoParalelo(u, v));
    fimParelelo = omp_get_wtime();
    tempoParelelo = fimParelelo - inicioParelelo;
    printf("Paralelo: %.4lf\n", tempoParelelo);
    speedup = tempoSerial / tempoParelelo;
    eficiencia = speedup / N_THREADS;
    printf("SPEEDUP: %.4lf\n", speedup);
    printf("EFICIENCIA: %.4lf\n", eficiencia);
    free(u);
    free(v);
    return 0;
}

int produtoInternoSerial(int *u, int *v)
{
    int soma = 0;
    for (int i = 0; i < TAM_V; i++)
    {
        soma += u[i] * v[i];
    }
    
    return soma;
}

int produtoInternoParalelo(int *u, int *v)
{
    int somaT = 0, i;
    #pragma omp parallel for reduction(+: somaT) default(none) shared(u, v) private(i) num_threads(N_THREADS)
    for (i = 0; i < TAM_V; i++)
    {
        somaT += u[i] * v[i];
    }
    
    return somaT;
}

void preencheVetores(int *u, int *v)
{
    for (int i = 0; i < TAM_V; i++)
    {
        u[i] = 1;
        v[i] = 1;
    }
    
}

void imprimeVetores(int *u, int *v)
{
    for (int i = 0; i < TAM_V; i++)
    {
        printf("u[%d] = %d\n", i, u[i]);
        printf("v[%d] = %d\n", i, v[i]);
    }
    
}