#ifndef ATIVIDADEPP_H
#define ATIVIDADEPP_H

#if defined(__MINGW32__) || defined(_MSC_VER)
#define limpar_input() fflush(stdin)
#define limpar_tela() system("cls")
#else
#define limpar_input() __fpurge(stdin)
#define limpar_tela() system("clear")
#endif

typedef struct
{
    int inicio;
    int fim;
} INTERVALO;
#define V_TAM (int)5e8 // tamanho vetor
#define M_TAM (int)1   // tamanho matriz
#define N_THREADS 8
#define N_ARGS 8

#define N 5                    // numeros de filosofos
#define V_ESQUERDA (i + N - 1) // vizinhos a esquerda
#define V_DIREITA (i + 1) % N  // vizinhos a direita
#define PENSANDO 0             // filosofo pensando
#define FAMINTO 1              // filosofo tentando pegar o garfo
#define COMENDO 2              // filosofo comendo
enum
{
    false,
    true
};

void *paraleloProdutoInterno(void *);
int produtoInterno(int *, int *);
void derivadaParcial(int[][M_TAM]);

void comendo(int);
void pensando(int);
void test(int);
void pegue_garfos(int);
void retorne_garfos(int);
void *filosofo(void *);


#endif