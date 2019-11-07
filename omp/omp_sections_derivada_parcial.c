#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM (int) 10

void derivadaParcialXY(int**, int**, int**);
void derivadParcialX(int**, int**);
void derivadParcialY(int**, int**);
void gerarMatriz(int**, int**, int**);
void salvarArquivo(char*, int**);

int main(){
    int **matriz, **matrizX, **matrizY;
    double inicio, fim;
    matriz = (int**)malloc(sizeof(int*) * TAM);
    matrizX = (int**)malloc(sizeof(int*) * TAM);
    matrizY = (int**)malloc(sizeof(int*) * TAM);
    for (int i = 0; i < TAM; i++)
    {
        matriz[i] = (int*)malloc(sizeof(int) * TAM);
        matrizX[i] = (int*)malloc(sizeof(int) * TAM);
        matrizY[i] = (int*)malloc(sizeof(int) * TAM);
    }
    
    gerarMatriz(matriz, matrizX, matrizY);
    inicio = omp_get_wtime();
    derivadaParcialXY(matriz, matrizX, matrizY);
    fim = omp_get_wtime();
    printf("TEMPO: %lf\n", fim-inicio);
    char nome[30], nome2[30];
    sprintf(nome, "matrizXSerial%d.txt", TAM);
    sprintf(nome2, "matrizYSerial%d.txt", TAM);
    salvarArquivo(nome, matrizX);
    salvarArquivo(nome2, matrizY);

    for (int i = 0; i < TAM; i++)
    {
        free(matriz[i]);
        free(matrizX[i]);
        free(matrizY[i]);
    }
    free(matriz);
    free(matrizX);
    free(matrizY);

    return 0;
}

void derivadaParcialXY(int **matriz, int ** matrizX, int **matrizY){
    #pragma omp parallel sections default(none) shared(matriz, matrizX, matrizY) num_threads(2)
    {
        #pragma omp section
        {
            derivadParcialX(matriz, matrizX);
        }
        #pragma omp section
        {
            derivadParcialY(matriz, matrizY);
        }
    }
    
    
}

void derivadParcialX(int **matriz, int **matrizX){
    int x = 0, y = 0,inf, sup;
    // #pragma omp parallel for collapse(2) default(none) shared(matriz, matrizX) private(x, y, inf, sup) num_threads(2)
    for (x = 0; x < TAM; x++)
    {
        for (y = 0; y < TAM; y++)
        {
            if(y == 0)
                printf("ThreadX%d\n",omp_get_thread_num());
            if (x - 1 < 0 )
            {
                inf = x + 1;
                sup = 0; //pegando o proprio elemento para substituir vizinho superior que não existe
            }
            else if (x + 1 == TAM)
            {
                sup = x - 1;
                inf = TAM - 1; //pegando o proprio elemento para substituir vizinho inferior que não existe
            }
            else
            { //caso onde todos os vizinhos existem
                sup = x - 1;
                inf = x + 1;
            }
            matrizX[x][y]= (matriz[inf][y]-matriz[sup][y])/2;
        }
        
    }
}

void derivadParcialY(int **matriz, int **matrizY){
    int x = 0, y = 0,dir, esq;
    // #pragma omp parallel for collapse(2) default(none) shared(matriz, matrizY) private(x, y, dir, esq) num_threads(2)
    for (x = 0; x < TAM; x++)
    {
        for (y = 0; y < TAM; y++)
        {
            if(y == 0)
                printf("ThreadY%d\n",omp_get_thread_num()); 
            if (y - 1 < 0 )
            {
                dir = y + 1;
                esq = 0; //pegando o proprio elemento para substituir vizinho esquerda que não existe
            }
            else if (y + 1 == TAM)
            {
                esq = y - 1;
                dir = TAM - 1; //pegando o proprio elemento para substituir vizinho direito que não existe
            }
            else
            { //caso onde todos os vizinhos existem
                esq = y - 1;
                dir = y + 1;
            }
            matrizY[x][y]=(matriz[dir][y]-matriz[esq][y])/2;
        }
        
    }
}

void gerarMatriz(int **mat1, int **mat2, int **mat3){
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            mat1[i][j] = j+1;
            mat2[i][j] = 0;
            mat3[i][j] = 0; 
        }
    }
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