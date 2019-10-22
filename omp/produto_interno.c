#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM (int)6e8

void produtoInterno(int *v, int *u)
{
    int somaT = 0;
    int somaP = 0;
    int i = 0;
    int num_threads = 0;
    #pragma omp parallel default(none) shared(somaT, v, u, num_threads) private(somaP, i) num_threads(10)
    {
        num_threads = omp_get_num_threads();
        #pragma omp for
        for (i = 0; i < TAM; i++)
        {
            somaP += u[i] * v[i];
        }
        // printf("Soma total %d: %d \n",omp_get_thread_num(), somaT);
        // printf("Soma parcial %d: %d\n", omp_get_thread_num(), somaP);
        #pragma omp atomic
        somaT += somaP;
        // printf("Soma total %d: %d \n",omp_get_thread_num(), somaT);
        // printf("Soma parcial da thread %d: %d\n", omp_get_thread_num(), sum);
    }
    printf("Totais de threads: %d\n", num_threads);
    printf("<u, v> = %d\n", somaT);
    // printf("Soma da thread %d: %d\n", omp_get_thread_num(), sum);
    

}

int main()
{
    int *u, *v, i;
    double inicio, fim;
    u = malloc(sizeof(int) * TAM);
    v = malloc(sizeof(int) * TAM);
    for (i = 0; i < TAM; i++)
    {
        u[i] = 1;
        v[i] = 1;
    }
    // for(i = 0; i < TAM; i++) printf("%d ", v[i]);
    inicio = omp_get_wtime();
    produtoInterno(v, u);
    fim = omp_get_wtime();
    printf("Tempo: %f segundos\n", fim -inicio);
    free(u);
    free(v);
    return 0;
}
