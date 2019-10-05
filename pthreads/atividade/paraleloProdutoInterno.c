#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "pthread.h"
#include "atividadepp.h"

int u[V_TAM];
int v[V_TAM];
int soma = 0;          //variavel compartilhada entre as threads
pthread_mutex_t trava; //mutex para a região critica

int main()
{
    double inicio, fim;
    pthread_t threads[N_THREADS];
    INTERVALO args[N_ARGS];

    for (int i = 0; i < V_TAM; i++) //inicializando os vetores
    {
        u[i] = 1;
        v[i] = 1;
    }

    for (int i = 0; i < N_ARGS; i++) //atribuindo intervalo para o vetor args
    {
        if (i == 0) //primeiro intervalo
        {
            args[i].inicio = 0;
            args[i].fim = V_TAM / N_ARGS;
        }
        else //restante dos intervalos
        {
            args[i].inicio = args[i - 1].fim;
            args[i].fim = args[i].inicio + (V_TAM / N_ARGS);
        }
    }
    //verificando intervalos
    if (args[N_ARGS - 1].fim != V_TAM)
    {
        printf("Distribuição de intervalo errada!\n");
        exit(-1);
    }
    inicio = omp_get_wtime();         //iniciando contagem de tempo
    pthread_mutex_init(&trava, NULL); //inicializando o mutex
    int c = 0;                        //contador para vetor args
    while (c != N_ARGS)               //percorrendo vetor de intervalos
    {
        for (int i = 0; i < N_THREADS; i++) //percorrendo vetor de threads
        {
            pthread_create(&threads[i], NULL, paraleloProdutoInterno, &args[c]); // criando threads para cada intervalo dos vetores
            c++;
        }
    }
    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL); //unindo threads com a main
    }
    fim = omp_get_wtime(); //finalizando a contagem de tempo
    printf("<u, v> = %d\n", soma);
    printf("Tempo: %lf s\n", fim - inicio);
    pthread_mutex_destroy(&trava); //desfazendo o mutex da região critica
    pthread_exit(NULL);
}

void *paraleloProdutoInterno(void *arg)
{
    INTERVALO *argumentos = arg;
    pthread_mutex_lock(&trava); //entrando na região critica
    for (int i = argumentos->inicio; i < argumentos->fim; i++)
        soma += u[i] * v[i];
    pthread_mutex_unlock(&trava); //saindo da região critica
    pthread_exit(NULL);
}