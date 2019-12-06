#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 100000
#define TOTAL_V 512
#define TOTAL_A 512

enum
{
	false,
	true
};

//tipo booleano
typedef int bool;

//Representação de aresta
typedef struct
{
	int u;
	int v;

} Aresta;

//Representação de vértice
typedef struct
{
	int id;
	bool visitado;
} Vertice;

void imprimeResultado(int *);
void dijkstraSerial(Vertice *, Aresta *, int *, Vertice *);
int encontraAresta(Vertice, Vertice, Aresta *, int *);
int minimo(int, int);
int pesoMin(int *, Vertice *);
int caminhoMin(Vertice *, int *);

int main(void)
{
	Vertice vertices[TOTAL_V];

	Aresta arestas[TOTAL_A];
	int pesos[TOTAL_A];

	//TESTE
	// Aresta arestas[TOTAL_A] = {{0, 1}, {0, 4}, {4, 3}, {1, 2}, {2 ,3}, {3,5}, {4, 2}, {1,3}};
	// int pesos[TOTAL_A] = {4, 1, 10, 5, 4, 1, 3, 5};

	// int i = 0;
	// for (i = 0; i < TOTAL_V; i++)
	// {
	// 	Vertice a = {.id = i, .visitado = false};
	// 	vertices[i] = a;
	// }
	//TESTE

	//GRAFO ALEATORIO
	srand(time(NULL));
	int i = 0;
	for(i = 0; i < TOTAL_V; i++)
	{
		Vertice a = { .id =(int) i, .visitado=false};
		vertices[i] = a;
	}

	for(i = 0; i < TOTAL_A; i++)
	{
		Aresta e = {.u = (int) rand()%TOTAL_V , .v = rand()%TOTAL_V};
		arestas[i] = e;
		pesos[i] = rand()%100;
	}
	//GRAFO ALEATORIO

	Vertice origem = {0, false};

	printf("OpenMP Resultados para grafo de %d Vertices e %d Arestas:\n", TOTAL_V, TOTAL_A);
	dijkstraSerial(vertices, arestas, pesos, &origem);
}

void salvarResultado(int *caminhosMin, char *nome)
{
	FILE *arq;
	arq = fopen(nome, "w");
	fprintf(arq, "Vértice\t\t Distância da origem\n");
	for (int i = 0; i < TOTAL_V; i++)
	{
		if(caminhosMin[i] == MAX)
			fprintf(arq, " \t\t \n%d \t\t INALCANÇÁVEL", i);
		else
			fprintf(arq, " \t\t \n%d \t\t %d", i, caminhosMin[i]);
	}
	fclose(arq);
	
}

//Imprime resultado
void imprimeResultado(int *caminhosMin)
{
	int i;
	printf("Vértice\t\t Distância da origem\n");
	for (i = 0; i < TOTAL_V; i++)
		if(caminhosMin[i] == MAX)
			printf(" \t\t \n%d \t\t INALCANÇÁVEL", i);
		else
			printf(" \t\t \n%d \t\t %d", i, caminhosMin[i]);
}

//Algoritmo dijkstra
void dijkstraSerial(Vertice *vertices, Aresta *arestas, int *pesos, Vertice *origem)
{
	double inicio, fim;
	origem->visitado = true;
	int caminhosMin[TOTAL_V];
	caminhosMin[(int)origem->id] = 0;

	int i, j;

	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].id != origem->id)
			caminhosMin[(int)vertices[i].id] = encontraAresta(*origem, vertices[i], arestas, pesos);
		else
			vertices[i].visitado = true;

	inicio = omp_get_wtime();
	for (j = 0; j < TOTAL_V; j++)
	{
		Vertice u;
		int vMin = caminhoMin(vertices, caminhosMin);
		u = vertices[vMin];
		for (i = 0; i < TOTAL_V; i++){
			if (vertices[i].visitado == false)
			{
				
				int peso_u_v = encontraAresta(u, vertices[i], arestas, pesos);
				caminhosMin[vertices[i].id] = minimo(caminhosMin[vertices[i].id], caminhosMin[u.id] + peso_u_v);

			}
		}
		
	}
	fim = omp_get_wtime();
	// imprimeResultado(caminhosMin, vertices);
	salvarResultado(caminhosMin, "dijkstraSerial.txt");
	printf("\nTempo: %f s\n", (fim - inicio));
}

//Encontra aresta que conecta u e v
int encontraAresta(Vertice u, Vertice v, Aresta *arestas, int *pesos)
{
	int i;
	for (i = 0; i < TOTAL_A; i++)
		if (arestas[i].u == u.id && arestas[i].v == v.id)
			return pesos[i];
	return MAX;
}

//minimo entre dois numeros
int minimo(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

//Percorre os vertices e encontra o menor peso do vertice
int pesoMin(int *caminhosMin, Vertice *vertices)
{
	int i;
	int menor = MAX;
	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].visitado == true)
			continue;
		else if (vertices[i].visitado == false && caminhosMin[i] < menor)
			menor = caminhosMin[i];
	return menor;
}

//encontra vertice com menor caminho
int caminhoMin(Vertice *vertices, int *caminhosMin)
{
	int i;
	int min = pesoMin(caminhosMin, vertices);

	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].visitado == false && caminhosMin[vertices[i].id] == min)
		{
			vertices[i].visitado = true;
			return i;
		}
}
