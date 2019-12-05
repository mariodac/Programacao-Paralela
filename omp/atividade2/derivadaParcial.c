#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM (int) 1e4
#define N_THREADS 16

void derivadaParcialSerial(int**, int**, int**);
void derivadaParcialParalelo(int**, int**, int**);
void gerarMatriz(int**, int**, int**);
void salvarArquivo(char*, int**);

int main()
{
    int **m, **mx, **my;
    double inicioSerial = 0, fimSerial = 0, inicioParalelo = 0, fimParalelo = 0, speedup = 0, eficiencia = 0, tempoSerial = 0, tempoParalelo = 0;
    m = (int**) malloc(sizeof(int*) * TAM);
    mx = (int**) malloc(sizeof(int*) * TAM);
    my = (int**) malloc(sizeof(int*) * TAM);
    for(int i = 0; i < TAM; i++)
    {
        m[i] = (int*) malloc(sizeof(int) * TAM);
        mx[i] = (int*) malloc(sizeof(int) * TAM);
        my[i] = (int*) malloc(sizeof(int) * TAM);
    }
    gerarMatriz(m, mx, my);
    inicioSerial = omp_get_wtime();
    derivadaParcialSerial(m, mx, my);
    fimSerial = omp_get_wtime();
    tempoSerial = fimSerial - inicioSerial;
    printf("Serial: %.4lf\n", tempoSerial);
    salvarArquivo("matrizSerialX.txt", mx);
    salvarArquivo("matrizSerialY.txt", my);
    gerarMatriz(m, mx, my);
    inicioParalelo = omp_get_wtime();
    derivadaParcialParalelo(m, mx, my);
    fimParalelo = omp_get_wtime();
    tempoParalelo = fimParalelo - inicioParalelo;
    speedup = tempoSerial / tempoParalelo;
    eficiencia = speedup / N_THREADS;
    printf("Paralelo: %.4lf\n", tempoParalelo);
    printf("SPEEDUP: %.4f\n", speedup);
    printf("EFICIENCIA: %.4f\n", eficiencia);
    salvarArquivo("matrizParaleloX.txt", mx);
    salvarArquivo("matrizParaleloY.txt", my);
    for(int i = TAM; i >= 0; i--)
        free(m[i]);
    free(m);
    return 0;
}

void derivadaParcialSerial(int **m, int **mx, int **my)
{
    int inf, sup, esq, dir;
    for (int x = 0; x < TAM; x++)
    {
        for (int y = 0; y < TAM; y++)
        {
            if (x - 1 < 0 && y - 1 < 0)
            {
                dir = y + 1;
                inf = x + 1;
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerdo que não existe
                sup = 0; //pegando o proprio elemento para substituir vizinho superior que não existe
            }
            else if (x + 1 == TAM && y + 1 == TAM)
            {
                esq = y - 1;
                sup = x - 1;
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
            }
            else if (x + 1 == TAM && y - 1 < 0)
            {
                sup = x - 1;
                dir = y + 1;
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = 0;         //pegando o proprio elemento para substituir vizinho esquerdo que não existe
            }
            else if (x - 1 < 0 && y + 1 == TAM)
            {
                inf = x + 1;
                esq = y - 1;
                sup = 0;         //pegando o proprio elemento para substituir vizinho superior que não existe
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
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
            else if (x + 1 == TAM)
            {
                sup = x - 1;
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = y - 1;
                dir = y + 1;
            }
            else if (y + 1 == TAM)
            {
                sup = x - 1;
                inf = x + 1;
                esq = y - 1;
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
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

void derivadaParcialParalelo(int **m, int **mx, int **my)
{
    int inf, sup, esq, dir, x, y;
    #pragma omp parallel for collapse(2) default(none) private(x, y, inf, sup, esq, dir) shared(m, mx, my) num_threads(N_THREADS)
    for (x = 0; x < TAM; x++)
    {
        for (y = 0; y < TAM; y++)
        {
            if (x - 1 < 0 && y - 1 < 0)
            {
                dir = y + 1;
                inf = x + 1;
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerdo que não existe
                sup = 0; //pegando o proprio elemento para substituir vizinho superior que não existe
            }
            else if (x + 1 == TAM && y + 1 == TAM)
            {
                esq = y - 1;
                sup = x - 1;
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
            }
            else if (x + 1 == TAM && y - 1 < 0)
            {
                sup = x - 1;
                dir = y + 1;
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerdo que não existe
            }
            else if (x - 1 < 0 && y + 1 == TAM)
            {
                inf = x + 1;
                esq = y - 1;
                sup = 0;         //pegando o proprio elemento para substituir vizinho superior que não existe
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
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
            else if (x + 1 == TAM)
            {
                sup = x - 1;
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = y - 1;
                dir = y + 1;
            }
            else if (y + 1 == TAM)
            {
                sup = x - 1;
                inf = x + 1;
                esq = y - 1;
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
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

void salvarArquivo(char *nome, int **m)
{
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
void gerarMatriz(int **matrizA, int **matrizB, int **matrizC)
{
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
        {
            matrizA[i][j] = j+1;
            matrizB[i][j] = 0;
            matrizC[i][j] = 0;
        }
    }
}