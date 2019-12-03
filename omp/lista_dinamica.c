#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4
int tamanho = 0;

typedef struct node
{
	long long data;
	struct node *next;
} Node;

void printList(Node *n)
{
	printf("Inicio da lista--\n");
	while (n != NULL)
	{
		printf(" %Ld\n", n->data);
		n = n->next;
	}
	printf("Fim da lista--\n");
}

void processList(Node *n)
{

	while (n != NULL)
	{
		operacao(n);
		n = n->next;
	}
}

void operacao(Node *n)
{
	long long i;
	for (i = 0; i < 0xFFFFFF; i++)
	{
	}
	n->data++;
}

void processaListaTask(Node *n)
{
#pragma omp parallel num_threads(NUM_THREADS)
	{

		#pragma omp single
		{
			while (n != NULL)
			{
				#pragma omp task
				operacao(n);
				n = n->next;
			}
		}
	}
}

void processaListaSingle(Node *n)
{
	#pragma omp parallel firstprivate(n)
	while (n != NULL)
	{
		#pragma omp single nowait
		operacao(n);
		n = n->next;
	}
}

void push(Node **head, int val)
{
	Node *n;
	n = malloc(sizeof(Node));
	n->data = val;
	n->next = *head;
	*head = n;
	tamanho++;
}

long int fibSerial(int n){
    if(n == 0)
        return 0;
    else if(n == 1)
        return 1;
    else
        return fibSerial(n - 1) + fibSerial(n - 2);
}

long int fibTask(int n)
{
		if(n == 0){
    	    return 0;}
    	else if(n == 1){
    	    return 1;}
    	else{
    	    return fibTask(n - 1) + fibTask(n - 2);}

    
}

int main()
{
	long long i;
	double inicio_serial, fim_serial, inicio_paralelo, fim_paralelo, tempo_serial, tempo_paralelo, speedup, eficiencia;
	Node *head = NULL;
	for (i = 0; i < 50; i++)
		push(&head, i);
	long long vetor[tamanho];

	printf("added nodes.\n");

	// printList(head);
	// SERIAL INICIO
	inicio_serial = omp_get_wtime();
	processList(head);
	fim_serial = omp_get_wtime();
	tempo_serial = fim_serial - inicio_serial;
	printf("SERIAL: %.3f\n", tempo_serial);
	// SERIAL FIM

	//PARALELO TASK INICIO
	inicio_paralelo = omp_get_wtime();
	processaListaTask(head);
	fim_paralelo = omp_get_wtime();
	tempo_paralelo = fim_paralelo - inicio_paralelo;
	printf("PARALELO COM TASK: %.3f\n", tempo_paralelo);
	speedup = tempo_serial / tempo_paralelo;
	printf("SPEEDUP TASK: %.3f\n", speedup);
	eficiencia = speedup/NUM_THREADS;
	printf("EFICIENCIA: %.3f\n", eficiencia);
	//PARALELO TASK FIM

	//PARALELO SINGLE NOWAIT INICIO
	inicio_paralelo = omp_get_wtime();
	processaListaSingle(head);
	fim_paralelo = omp_get_wtime();
	tempo_paralelo = fim_paralelo - inicio_paralelo;
	printf("PARALELO COM SINGLE: %.3f\n", tempo_paralelo);
	speedup = tempo_serial / tempo_paralelo;
	printf("SPEEDUP SINGLE: %.3f\n", speedup);
	eficiencia = speedup/NUM_THREADS;
	printf("EFICIENCIA: %.3f\n", eficiencia);
	//PARALELO SINGLE NOWAIT FIM
	// printList(head);

	return 0;
}
