#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "omp.h"
#define TAM 16000
#define N_THREADS 2
#define N_ARGS 2

 int mat[TAM][TAM];
 int mat_soma[TAM][TAM];

typedef struct {
    int inicio;
    int fim;
}ARG;

void *operacaoMatriz(void*);
void salvarArquivo(char*,  int[][TAM]);
void *matrizSoma(void*);

int main(){
    double start, end;

    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++){
            mat[i][j] = 0;
            mat_soma[i][j] = 0;
        }
    /* threads e argumentos sem uso de vetores */
    // pthread_t t1, t2;
    // struct args_struct arg1, arg2;
    // arg1.inicio = 0;
    // arg1.fim = TAM / N_ARGS;
    // arg2.inicio = arg1.fim;
    // arg2.fim = arg2.inicio + (TAM / N_ARGS);
    // pthread_create(&t1, NULL, operacaoMatriz, (void*)&arg1);
    // pthread_create(&t2, NULL, operacaoMatriz, (void*)&arg2);
    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    /* threads e argumentos sem uso de vetores */

    /* usando vetor para threads e argumentos */
    pthread_t threads[N_THREADS];
    ARG args[N_ARGS];
    for (int i = 0; i < N_ARGS; i++)
    {
        if(TAM % 2 != 0){
            
            if(i == 0){
                args[i].inicio = 0;
                args[i].fim = TAM+1 / N_ARGS;
            }
            else{
                args[i].inicio = args[i-1].fim;
                args[i].fim = args[i].inicio + (TAM+1/N_ARGS);
            }
        }
        else if(TAM % N_ARGS != 0){
            if(i == 0){
                args[i].inicio = 0;
                args[i].fim = (TAM / N_ARGS);
            }
            else if(i == N_ARGS-1){
                args[i].inicio = args[i-1].fim;
                args[i].fim = TAM;//args[i].inicio + ((TAM / N_ARGS));
            }
            else{
                args[i].inicio = args[i-1].fim;
                args[i].fim = args[i].inicio + ((TAM / N_ARGS));
            }
        }
        else{
            if(i == 0){
                args[i].inicio = 0; //inicio intervalo
                args[i].fim = TAM / N_ARGS; //fim do intervalo
            }
            else{
                args[i].inicio = args[i-1].fim; //continuando intervalo a partir da ultimo fim do intervalo
                args[i].fim = args[i].inicio + (TAM/N_ARGS); //incrementando fim deste intervalo com seu inicio mais o pedaço
            }
        }
    }
    if(args[N_ARGS-1].fim != TAM){//verificação de intervalo
        printf("ERRO!\n");
        exit(-1);
    } 
    int c = 0;
    start = omp_get_wtime();
    while (c != N_ARGS)
    {
        for (int i = 0; i < N_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, operacaoMatriz, &args[c]); //criando thread para preenchimento da matriz cada uma com seu intervalo diferente
            c++;
            // pthread_join(threads[i], NULL);
        }
    }
    
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL); //unindo todas as threads neste ponto
    c = 0;
    while (c != N_ARGS)
    {
        for (int i = 0; i < N_THREADS; i++)
        {
            pthread_create(&threads[i], NULL, matrizSoma, &args[c]); //utilizando mesmas threads usadas antes para realizar soma 4-vizinhança pois já terminaram o seu trabalho
            c++;
        }
    }
    for (int i = 0; i < N_THREADS; i++)
        pthread_join(threads[i], NULL); //unindo todas as threads neste ponto
    
    end = omp_get_wtime(); //terminando contagem de tempo
    printf("Tempo: %lf segundos\n", end - start);
    /* usando vetor para threads e argumentos */
    
    salvarArquivo("matrizThread.txt", mat);//salvando arquivo para primeira matriz
    salvarArquivo("matrizThreadSoma.txt", mat_soma);//salvando arquivo para matriz soma

    pthread_exit(NULL); //encerrando a thread
}

void *matrizSoma(void *arguments){
    ARG *args = arguments;
    int sup, inf, esq, dir;
    for (int i = args->inicio; i < args->fim; i++)//divisão de dominio em linhas
    {
        for (int j = 0; j < TAM; j++)
        {
            if(i - 1 < 0 && j - 1 < 0){
                dir = j + 1;
                inf = i + 1;
                esq = 0;//pegando o proprio elemento para substituir vizinho esquerdo que não existe
                sup = 0;//pegando o proprio elemento para substituir vizinho superior que não existe
            }
            else if(i + 1 == TAM && j + 1 == TAM){
                esq = j - 1;
                sup = i - 1;
                dir = TAM - 1;//pegando o proprio elemento para substituir vizinho direito que não existe
                inf = TAM - 1;//pegando o proprio elemento para substituir vizinho inferior que não existe
            }
            else if(i + 1 == TAM && j - 1 < 0){
                sup = i - 1;
                dir = j + 1;
                inf = TAM - 1;//pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = 0;//pegando o proprio elemento para substituir vizinho esquerdo que não existe
            }
            else if(i - 1 < 0 && j + 1 == TAM){
                inf = i + 1;
                esq = j - 1;
                sup = 0;//pegando o proprio elemento para substituir vizinho superior que não existe
                dir = TAM - 1;//pegando o proprio elemento para substituir vizinho direito que não existe
            }
            else if(j - 1 < 0){
                sup = i - 1;
                inf = i + 1;
                esq = 0;//pegando o proprio elemento para substituir vizinho esquerdo que não existe
                dir = j + 1;
            }
            else if(i - 1 < 0){
                sup = 0;//pegando o proprio elemento para substituir vizinho superior que não existe
                inf = i + 1;
                esq = j - 1;
                dir = j + 1; 
            }
            else if(i + 1 == TAM){
                sup = i - 1;
                inf = TAM - 1;  //pegando o proprio elemento para substituir vizinho inferior que não existe
                esq = j - 1;
                dir = j + 1;
            }
            else if(j + 1 == TAM){
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = TAM - 1;//pegando o proprio elemento para substituir vizinho direito que não existe
            }
            else{//caso onde todos os vizinhos existem
                sup = i - 1;
                inf = i + 1;
                esq = j - 1;
                dir = j + 1;
            }
            mat_soma[i][j] = mat[inf][j] + mat[sup][j] + mat[i][esq] + mat[i][dir];//soma da 4-vizinhaça
        }
    }
    pthread_exit(NULL); //encerrando a thread
}

void *operacaoMatriz(void *arguments){
    ARG *args = arguments;
    // printf("Inicio: %d Fim: %d\n", args->inicio, args->fim);
    for(int i = args->inicio; i < args->fim; i++){ //divisão de dominio em linhas
        for (int j = 0; j < TAM; j++)
            mat[i][j] = (i + j) - (i * j);
    }
    pthread_exit(NULL); //encerrando a thread
}

void salvarArquivo(char *nome,  int m[][TAM]){
    FILE *pont_arq;
    pont_arq = fopen(nome, "w");//abertura/criação do arquivo 
    for (int i = 0; i < TAM; i++)
    {
        for (int j = 0; j < TAM; j++)
            fprintf(pont_arq, "%d\t", m[i][j]);//escrita no arquivo
        fprintf(pont_arq, "\n");
        
    }
    fclose(pont_arq);//fechamento do arquivo
}