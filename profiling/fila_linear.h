#ifndef FILA_LINEAR_H
#define FILA_LINEAR_H

#define TAM 100000000

struct filaLinear{
    int nodes[TAM];
    char nodesElementos[TAM];
    int inicio;
    int fim;
    int numNodes;
    int vetPriori[TAM];
};


#endif 
//DEFINIÇÕES DE IMPLEMENTAÇÃO
/*
- Uma fila vazia está com início = 0 e fim = -1

*/