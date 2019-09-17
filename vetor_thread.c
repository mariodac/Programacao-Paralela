#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"

#define TAM 1000000
#define N_THREADS 4
#define N_ARGS 4

int v[TAM];

struct args_struct{
    int inicio;
    int fim;
};

void *calc(void *arguments){
    struct args_struct *args = arguments;
    printf("Inicio: %d Fim: %d\n", args->inicio, args->fim);
    for (int i = args->inicio; i < (args->fim); i++)
    {
        // printf("%d -> ", i);
        v[i] = (i + i) - (i * i);
        // sleep(1);
        // printf("%d ", v[i]);
        // printf("\n");
    }
    pthread_exit(NULL);
}

int main(){

    clock_t start, end;
    double cpu_time_used;
    start = clock();    
    
    FILE *pont_arq;
    pont_arq = fopen("vetorThread.txt", "w");

    for(int i = 0; i < TAM; i++)
        v[i] = 0;

    /* usando vetor para threads e argumentos */
    pthread_t threads[N_THREADS];
    struct args_struct args[N_ARGS];
    for (int i = 0; i < N_ARGS; i++)
    {
        if(i == 0){
            args[i].inicio = 0;
            args[i].fim = TAM / N_ARGS;
        }
        else{
            args[i].inicio = args[i-1].fim;
            args[i].fim = args[i].inicio + (TAM/N_ARGS);
        }
    }
    int c = 0;
    while (c != N_ARGS)
    {
        for (int i = 0; i < N_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, calc, &args[c]);
            c++;
            pthread_join(threads[i], NULL);
        }
    }
    // for (int i = 0; i < N_THREADS; i++)
    // {
    //     pthread_join(threads[i], NULL);
    // }
    /* usando vetor para threads e argumentos */

    /* threads e argumentos sem uso de vetores */
    // pthread_t t1, t2;
    // struct args_struct args1, args2, args3, args4;
    // args1.inicio = 0;
    // args1.fim = TAM / N_ARGS;
    // args2.inicio = args1.fim;
    // args2.fim = args2.inicio + (TAM/N_ARGS);
    // args3.inicio = args2.fim;
    // args3.fim = args3.inicio + (TAM/N_ARGS);
    // args4.inicio = args3.fim;
    // args4.fim = args4.inicio + (TAM/N_ARGS);
    // pthread_create(&t1, NULL, calc, (void*)&args1); //thread t1 args1
    // pthread_create(&t2, NULL, calc, (void*)&args2); //thread t2 args2
    
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);

    // pthread_create(&t1, NULL, calc, &args3);
    // pthread_create(&t2, NULL, calc, &args4);

    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    /* threads e argumentos sem uso de vetores */
    
    for (int i = 0; i < TAM; i++)
        fprintf(pont_arq, "%d\n", v[i]);
    fclose(pont_arq);
    

    end = clock();
    cpu_time_used = ((double) (end - start)) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo: %.2f ms\n", cpu_time_used);
    pthread_exit(NULL);
}
