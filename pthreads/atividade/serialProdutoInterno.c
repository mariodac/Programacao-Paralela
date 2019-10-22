#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "atividadepp.h"

int *u;
int *v;


int main()
{
    u = malloc(sizeof(int)*V_TAM);
    v = malloc(sizeof(int)*V_TAM);
    double inicio, fim;
    for (int i = 0; i < V_TAM; i++)
    {
        u[i] = 1;
        v[i] = 1;
    }
    inicio = omp_get_wtime();
    int produto = produtoInterno(u, v);
    fim = omp_get_wtime();
    printf("<u, v> = %d\n", produto);
    printf("Tempo: %f s\n", fim - inicio);
    free(u);
    free(v);
    return 0;
}

int produtoInterno(int *u, int *v)
{
    int soma = 0;
    for (int i = 0; i < V_TAM; i++)
    {
        soma += u[i] * v[i];
    }
    return soma;
}