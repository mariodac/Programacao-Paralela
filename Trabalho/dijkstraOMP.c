#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 100000
#define TOTAL_V 2048
#define TOTAL_A 2048 * 2
#define N_THREADS 8

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

void preencheGrafo(Vertice*, Aresta*, int*);
void preencheGrafoAleatorio(Vertice*, Aresta*, int*);
void salvarResultado(int *, char *);
void imprimeResultado(int *);
int *dijkstraOMP(Vertice *, Aresta *, int *, Vertice *);
int encontraAresta(Vertice, Vertice, Aresta *, int *);
int minimo(int, int);
int pesoMin(int *, Vertice *);
int caminhoMin(Vertice *, int *);

int main(void)
{
	Vertice vertices[TOTAL_V];

	//TESTE
	/* Aresta arestas[TOTAL_A] = {{0, 1}, {0, 4}, {4, 3}, {1, 2}, {2 ,3}, {3,5}, {4, 2}, {1,3}};
	int pesos[TOTAL_A] = {4, 1, 10, 5, 4, 1, 3, 5};

	int i = 0;
	for (i = 0; i < TOTAL_V; i++)
	{
		Vertice a = {.id = i, .visitado = false};
		vertices[i] = a;
	} */
	//TESTE

	Aresta arestas[TOTAL_A];
	int pesos[TOTAL_A];
	preencheGrafoAleatorio(vertices, arestas, pesos);
	int *caminhosMin;
	Vertice origem = {0, false};
	double inicio, fim;
	printf("OpenMP Resultados para grafo de %d Vertices e %d Arestas com %d Threads:\n", TOTAL_V, TOTAL_A, N_THREADS);
	inicio = omp_get_wtime();
	caminhosMin = dijkstraOMP(vertices, arestas, pesos, &origem);
	fim = omp_get_wtime();
	salvarResultado(caminhosMin, "OMP_dijkstra.txt");
	printf("Tempo: %.4f\n", (fim - inicio));
}

void preencheGrafoAleatorio(Vertice *vertices, Aresta *arestas, int *pesos)
{
	int i =0;
	srand(time(NULL));
	for(i = 0; i < TOTAL_V; i++)
	{
		Vertice v = { .id =(int) i, .visitado=false};
		vertices[i] = v;
	}
	for(i = 0; i < TOTAL_A; i++)
	{
		Aresta e = {.u = (int) rand()%TOTAL_V , .v = rand()%TOTAL_V};
		arestas[i] = e;
		pesos[i] = rand()%100;
	}
}
//para gerar grafo igual ao do serial
void preencheGrafo(Vertice *vertices, Aresta *arestas, int *pesos)
{
	int i = 0, j, k;
	for(i = 0; i < TOTAL_V; i++)
	{
		Vertice v = { .id =(int) i, .visitado=false};
		vertices[i] = v;
	}
	j = TOTAL_V;
	k = TOTAL_A/4;
	i = 0;
	while (i < TOTAL_A)
	{
		
		j = j/2;
		k = k/2;
		if(j != k){
			Aresta a = {.u = j, .v = k};
			arestas[i] = a;
			pesos[i] = i+2;
		}
		if(j == 0 && k == 0){
			k = TOTAL_A/2;
			j = TOTAL_A/4;
			Aresta a = {.u = k, .v = j};
			arestas[i] = a;
			pesos[i] = i+2;
		}
		
		i++;
	}
}
void salvarResultado(int *caminhosMin, char *nome)
{
	FILE *arq;
	arq = fopen(nome, "w");
	fprintf(arq, "Vértice\t\t Distância da origem\n");
	for (int i = 0; i < TOTAL_V; i++)
		fprintf(arq, " \t\t \n%d \t\t %d", i, caminhosMin[i]);
	fclose(arq);
}

//Imprime resultado
void imprimeResultado(int *caminhosMin)
{
	int i;
	printf("Vértice\t\t Distância da origem\n");
	for (i = 0; i < TOTAL_V; i++)
		if (caminhosMin[i] == MAX)
			printf(" \t\t \n%d \t\t INALCANÇÁVEL", i);
		else
			printf(" \t\t \n%d \t\t %d", i, caminhosMin[i]);
}

//Algoritmo dijkstra
int *dijkstraOMP(Vertice *vertices, Aresta *arestas, int *pesos, Vertice *origem)
{

	origem->visitado = true;
	int *caminhosMin = malloc(sizeof(int) * TOTAL_V);
	caminhosMin[(int)origem->id] = 0;

	int i, j;

	#pragma omp parallel for private(i) schedule(guided) num_threads(N_THREADS)
	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].id != origem->id)
			caminhosMin[(int)vertices[i].id] = encontraAresta(*origem, vertices[i], arestas, pesos);
		else
			vertices[i].visitado = true;

	Vertice u;
	int vMin;
	for (j = 0; j < TOTAL_V; j++)
	{
		vMin = caminhoMin(vertices, caminhosMin);
		u = vertices[vMin];
		#pragma omp parallel for schedule(guided) private(i)
		for (i = 0; i < TOTAL_V; i++)
		{
			if (vertices[i].visitado == false)
			{
				int peso_u_v = encontraAresta(u, vertices[i], arestas, pesos);
				caminhosMin[vertices[i].id] = minimo(caminhosMin[vertices[i].id], caminhosMin[u.id] + peso_u_v);
			}
		}
	}
	// imprimeResultado(caminhosMin, vertices);
	return caminhosMin;
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
