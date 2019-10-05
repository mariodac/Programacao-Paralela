#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define TAM 1e8

int u[(int)TAM];
int v[(int)TAM];

int produtoInterno(int *, int *);

int main()
{
    double inicio, fim;
    for (int i = 0; i < TAM; i++)
    {
        u[i] = 1;
        v[i] = 1;
    }
    inicio = omp_get_wtime();
    int produto = produtoInterno(u, v);
    fim = omp_get_wtime();
    printf("<u, v> = %d\n", produto);
    printf("Tempo: %f s\n", fim - inicio);
    return 0;
}

int produtoInterno(int *u, int *v)
{
    int soma = 0;
    for (int i = 0; i < TAM; i++)
    {
        soma += u[i] * v[i];
    }
    return soma;
}