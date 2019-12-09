#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>


#define MAX 100000
//total de vertices
#define TOTAL_V 512
//total de arestas
#define TOTAL_A 512*2

//enumerando os valores booleano
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
void imprimeResultado(int *);
int* dijkstraSerial(Vertice*, Aresta*, int*, Vertice *);
int encontraAresta(Vertice, Vertice, Aresta*, int *);
int minimo(int, int);
int pesoMin(int *, Vertice *);
int caminhoMin(Vertice *, int *);
void salvarResultado(int*, char*);

int main(void)
{
	Vertice vertices[TOTAL_V];

	//TESTE
	/* Aresta  arestas[TOTAL_A] = {{0, 1}, {0, 4}, {4, 3}, {1, 2}, {2 ,3}, {3,5}, {4, 2}, {1,3}};
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
	//setando vertice de origem
	Vertice origem = {0, false};
	double inicio, fim;
	printf("Serial Resultados para grafo de %d Vertices e %d Arestas:\n", TOTAL_V, TOTAL_A);
	//inicio da contagem de tempo
	inicio = omp_get_wtime();
	caminhosMin = dijkstraSerial(vertices, arestas, pesos, &origem);
	//final da contagem de tempo
	fim = omp_get_wtime();
	printf("Tempo: %.4f\n", fim-inicio);
	salvarResultado(caminhosMin, "Serial_dijkstra.txt");
}

//preenche grafo com arestas e pesos aleatorios
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
//para gerar grafo igual ao do OMP
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
//salva resultado do algoritmo dijkstra em arquivo.txt
void salvarResultado(int *caminhosMin, char *nome)
{
	FILE *arq;
	arq = fopen(nome, "w");
	fprintf(arq, "Vértice\t\t Distância da origem\n");
	for (int i = 0; i < TOTAL_V; i++)
		fprintf(arq, " \t\t \n%d \t\t %d", i, caminhosMin[i]);
	fclose(arq);
	
}

//Imprime resultado dos caminhos minimos na tela
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
int* dijkstraSerial(Vertice *vertices, Aresta *arestas, int *pesos, Vertice *origem)
{
	origem->visitado = true;
	//vetor que armazeno as distancias minima para cada vertice a partir da origem
	int *caminhosMin = malloc(sizeof(int) * TOTAL_V);
	//distancia 0 para vertice de origem
	caminhosMin[(int)origem->id] = 0;

	int i, j;
	//visita vertices a partir da origem
	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].id != origem->id)
			caminhosMin[(int)vertices[i].id] = encontraAresta(*origem, vertices[i], arestas, pesos);
		else
			vertices[i].visitado = true;

	for (j = 0; j < TOTAL_V; j++)
	{
		Vertice u;
		//encontrando o indice do vertice com caminho minimo
		int vMin = caminhoMin(vertices, caminhosMin);
		u = vertices[vMin];
		for (i = 0; i < TOTAL_V; i++){
			if (vertices[i].visitado == false)
			{
				//peso da aresta que conecta u e vertice[i]
				int peso_u_v = encontraAresta(u, vertices[i], arestas, pesos);
				//setando a distacia minima para o vertic[i]
				caminhosMin[vertices[i].id] = minimo(caminhosMin[vertices[i].id], caminhosMin[u.id] + peso_u_v);
			}
		}
		
	}
	// imprimeResultado(caminhosMin, vertices);
	return caminhosMin;
}

//Encontra aresta que conecta u e v e retorna o peso da aresta que conecta u e v
int encontraAresta(Vertice u, Vertice v, Aresta *arestas, int *pesos)
{
	int i;
	for (i = 0; i < TOTAL_A; i++)
		// se o vertice u da aresta atual é igual ao id do vertice u e vertice v da aresta atual é igual ao id do vertice v
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

//Percorre os vertices e encontra o menor distancia para cada vertice
int pesoMin(int *caminhosMin, Vertice *vertices)
{
	int i;
	// armazena distancia minima do vertice
	int menor = MAX;
	for (i = 0; i < TOTAL_V; i++)
		// se o vértice já foi visitado passa para próxima iteração
		if (vertices[i].visitado == true)
			continue;
		// só percorre os vértices que não foram visitados e tem uma distancia menor
		else if (vertices[i].visitado == false && caminhosMin[i] < menor)
			// armazenando a menor distancia até esse vertice
			menor = caminhosMin[i];
	return menor;
}

//encontra vertice com menor caminho
int caminhoMin(Vertice *vertices, int *caminhosMin)
{
	int i;
	// pega o distancia minima dos vertice
	int min = pesoMin(caminhosMin, vertices);
	// percorre vertices que não foram visitados e encontra o vertice que possue essa aresta com a distancia minima
	for (i = 0; i < TOTAL_V; i++)
		if (vertices[i].visitado == false && caminhosMin[vertices[i].id] == min)
		{
			vertices[i].visitado = true;
			// retorna o id do vertice
			return i;
		}
}
