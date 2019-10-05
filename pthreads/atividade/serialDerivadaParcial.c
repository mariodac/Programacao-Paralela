#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "atividadepp.h"

int m[M_TAM][M_TAM];
int mx[M_TAM][M_TAM];
int my[M_TAM][M_TAM];

int main()
{
    double inicio, fim;
    for (int i = 0; i < M_TAM; i++)
    {
        for (int j = 0; j < M_TAM; j++)
        {
            m[i][j] = j+1;
            mx[i][j] = 0;
            my[i][j] = 0;
        }
    }
    inicio = omp_get_wtime();
    derivadaParcial(m);
    fim = omp_get_wtime();

    printf("Tempo: %lf s\n", fim - inicio);
    salvarArquivo("serialMatriz_x.txt", mx);
    salvarArquivo("serialMatriz_y.txt", my);
    
    return 0;
}

void derivadaParcial(int m[][M_TAM])
{
    int inf, sup, esq, dir;
    for (int x = 0; x < M_TAM; x++)
    {
        for (int y = 0; y < M_TAM; y++)
        {
            if (x - 1 < 0 && y - 1 < 0)
            {
                dir = y + 1;
                inf = x + 1;
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerdo que não existe
                sup = 0; //pegando o proprio elemento para substituir vizinho superior que não existe
            }
            else if (x + 1 == M_TAM && y + 1 == M_TAM)
            {
                esq = y - 1;
                sup = x - 1;
                dir = M_TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
                inf = M_TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
            }
            else if (x + 1 == M_TAM && y - 1 < 0)
            {
                sup = x - 1;
                dir = y + 1;
                inf = M_TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = 0;         //pegando o proprio elemento para substituir vizinho esquerdo que não existe
            }
            else if (x - 1 < 0 && y + 1 == M_TAM)
            {
                inf = x + 1;
                esq = y - 1;
                sup = 0;         //pegando o proprio elemento para substituir vizinho superior que não existe
                dir = M_TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
            }
            else if (y - 1 < 0)
            {
                sup = x - 1;
                inf = x + 1;
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerdo que não existe
                dir = y + 1;
            }
            else if (x - 1 < 0)
            {
                sup = 0; //pegando o proprio elemento para substituir vizinho superior que não existe
                inf = x + 1;
                esq = y - 1;
                dir = y + 1;
            }
            else if (x + 1 == M_TAM)
            {
                sup = x - 1;
                inf = M_TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = y - 1;
                dir = y + 1;
            }
            else if (y + 1 == M_TAM)
            {
                sup = x - 1;
                inf = x + 1;
                esq = y - 1;
                dir = M_TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
            }
            else
            { //caso onde todos os vizinhos existem
                sup = x - 1;
                inf = x + 1;
                esq = y - 1;
                dir = y + 1;
            }
            mx[x][y]= (m[inf][y]-m[sup][y])/2;
            my[x][y]=(m[x][dir]-m[x][esq])/2;
        }
    }
}

void salvarArquivo(char *nome, int m[][M_TAM]){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w"); //abertura/criação do arquivo 
    for (int i = 0; i < M_TAM; i++)
    {
        for (int j = 0; j < M_TAM; j++)
            fprintf(pont_arq, "%d\t", m[i][j]); //escrita no arquivo
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq); //fechamento do arquivo
}