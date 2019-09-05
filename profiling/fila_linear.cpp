#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "fila_linear.h"

using namespace std;

void fila_remove_com_prioridade(filaLinear*, int, char);
void fila_insere_sem_prioridade(filaLinear*, int, char);
bool fila_prenche_aletorio(filaLinear*);
void fila_exibir(filaLinear*);
filaLinear* criarFila();
bool fila_vazia(filaLinear*);
bool fila_cheia(filaLinear*);
bool enfileirar(filaLinear*,int);
int select_op();
void inicializaFilaVazia(filaLinear*);
void get_inicio(filaLinear*);
void get_fim(filaLinear*);
bool retiraInicio_sem_ajustes(filaLinear*);
bool retira_inicio_com_ajustes(filaLinear*);
void esvaziar(filaLinear*);


int main() {
    filaLinear *MinhaFila = criarFila();
    int op;
    inicializaFilaVazia(MinhaFila);
    cout << "\nFila inicializada vazia\n";
    do{
        op = select_op();
        switch (op){
            case 0:
                cout << "\n\nSaindo do programa\n\n\n";
                break;
                
            case 1:
                system("clear");
                if(fila_vazia(MinhaFila))
                    cout << "\nFila vazia\n";
                else
                    fila_exibir(MinhaFila);
                break;
                
            case 2:
                system("clear");
                if(fila_vazia(MinhaFila))
                    cout << "\nFila vazia\n";
                else
                    cout << "\nFila não vazia\n";
                break;
                
            case 3:
                system("clear");
                if(fila_cheia(MinhaFila))
                    cout<<"\nFila cheia\n";
                else
                    cout<<"\nFila não cheia\n";
                break;
                
            case 4:
                system("clear");
                int i;
                cout << "\nDigite o elemento a incluir na fila \n";
                cin>>i;
                enfileirar(MinhaFila,i);
                break;
                
            case 5:
                system("clear");
                retiraInicio_sem_ajustes(MinhaFila);
                break;
                
            case 6:
                system("clear");
                if(fila_vazia(MinhaFila))
                    cout<<"\nFila vazia\n";
                else
                    get_fim(MinhaFila);
                break;
                
            case 7:
                system("clear");
                if(fila_vazia(MinhaFila))
                    cout<<"\nFila vazia\n";
                else
                    get_inicio(MinhaFila);
                break;
            case 8:
                system("clear");
                fila_insere_sem_prioridade(MinhaFila,2,'a');
                break;
            case 9:
                system("clear");
                fila_prenche_aletorio(MinhaFila);
                break;
            case 10:
                system("clear");
                esvaziar(MinhaFila);
                break;
            default:
                system("clear");
                cout<<"\n\nOpção inválida\n\n";
                break;
        }
    }while(op != 0);
    
    return 0;
}

void fila_insere_sem_prioridade(filaLinear* ptrAuxFila, int p, char e){
    if(fila_cheia(ptrAuxFila) == true){
        cout << "\nA fila está cheia\n";
    }
    else{
        
        ptrAuxFila->fim++;
        ptrAuxFila->nodesElementos[ptrAuxFila->fim] = e;
        ptrAuxFila->vetPriori[ptrAuxFila->fim] = p;
        ptrAuxFila->numNodes ++;
        cout <<"\nElemento adicionado na fila\n";
    }
}

void fila_remove_com_prioridade(filaLinear* ptrAuxFila, int p, char e){
    int ps;
    if(fila_vazia(ptrAuxFila)){
        cout << "\nFila vazia\n";
    }
    else{
        for(int i = 0; i < ptrAuxFila->numNodes-1; i++){
            if(ptrAuxFila->vetPriori[i] > ptrAuxFila->vetPriori[i+1]){
                ps = i;
                ptrAuxFila->nodes[ptrAuxFila->inicio] = 0; // opcional para remover lixo
                ptrAuxFila->inicio ++;
                ptrAuxFila->numNodes --;
                cout<<"\nElemento "<<ptrAuxFila->inicio<<" retirado com sucesso\n";
            }
        }

    }
}


bool fila_prenche_aletorio(filaLinear* ptrAuxFila){
    for(int i = 0; i<TAM; i++){
        if(fila_cheia(ptrAuxFila) == true){
            cout << "\nA fila está cheia\n";
            return false;
        }
        else{
            int a = rand() % 101;
            ptrAuxFila->fim++;
            ptrAuxFila->nodes[ptrAuxFila->fim] = a;
            ptrAuxFila->numNodes ++;
        }
    }
    cout << TAM << " Elementos adicionados na fila\n";
    return true;
}
void fila_exibir(filaLinear* ptrAuxFila){
    int i;
    for(i = 0; i<TAM; i++){
        if(!fila_vazia(ptrAuxFila)){
        cout <<"\nElemento "<<ptrAuxFila->fim<<":"<<ptrAuxFila->nodes[ptrAuxFila->fim];
        ptrAuxFila->fim--;
        }
        else
            cout<<"\nErro\n";
    }
}

void get_fim(filaLinear* ptrAuxFila){
    cout << "\nFim:\n" <<ptrAuxFila->nodes[ptrAuxFila->fim] << "\n\n";
}

void get_inicio(filaLinear* ptrAuxFila){
    cout << "\nInicio:\n" << ptrAuxFila->nodes[ptrAuxFila->inicio] << "\n\n";
}
filaLinear* criarFila(){
    filaLinear *ptrAuxFila;
    ptrAuxFila = (filaLinear*) malloc(sizeof(filaLinear));
    return ptrAuxFila;
}

void inicializaFilaVazia(filaLinear *ptrAuxFila){
    ptrAuxFila->inicio = 0;
    ptrAuxFila->fim = -1;
    ptrAuxFila->numNodes = 0;
}

bool fila_vazia(filaLinear *ptrAuxFila){
    if(ptrAuxFila->fim < ptrAuxFila->inicio)
        return true;
    else
        return false;
}

bool fila_cheia(filaLinear *ptrAuxFila){
    if(ptrAuxFila->numNodes == TAM)
        return true;
    else
        return false;
}

bool enfileirar(filaLinear *ptrAuxFila, int a){
    if(fila_cheia(ptrAuxFila) == true){
        cout << "\nA fila está cheia\n";
        return true;
    }
    else{
        
        ptrAuxFila->fim++;
        ptrAuxFila->nodes[ptrAuxFila->fim] = a;
        ptrAuxFila->numNodes ++;
        cout <<"\nElemento adicionado na fila\n";
        return false;
    }
}


bool retiraInicio_sem_ajustes(filaLinear *ptrAuxFila){
    if(fila_vazia(ptrAuxFila)){
        cout << "\nFila vazia\n";
        return true;
    }
    else{
        ptrAuxFila->nodes[ptrAuxFila->inicio] = 0; // opcional para remover lixo
        ptrAuxFila->inicio ++;
        ptrAuxFila->numNodes --;
        cout<<"\nElemento "<<ptrAuxFila->inicio<<" retirado com sucesso\n";
        return false;
    }
}

bool retira_inicio_com_ajustes(filaLinear *ptrAuxFila){
    int k;
    if(!fila_vazia(ptrAuxFila)){
    //ptrAuxFila->nodes[ptrAuxFila->inicio] = 0;
    //ptrAuxFila->inicio ++;
    //ptrAuxFila->numNodes --;
    for (k=0; k < ptrAuxFila->fim;k++){
        //ptrAuxFila->nodes[ptrAuxFila];
    }
    
    }
}

void esvaziar(filaLinear* ptrAuxFila){
    while (!fila_vazia(ptrAuxFila))
    {
        retiraInicio_sem_ajustes(ptrAuxFila);
    }
    cout << "Fila esvaziada\n";
    
}


int select_op(){
    int op;
    cout << "\n Digite a opção \n";
    cout << "\n 0 \t- Encerrar programa \n";
    cout << "\n 1 \t- Exibe todos elementos da fila \n";
    cout << "\n 2 \t- Verifica se fila está vazia \n";
    cout << "\n 3 \t- Verifica se fila está cheia \n";
    cout << "\n 4 \t- Adicionar elemento na fila \n";
    cout << "\n 5 \t- Remover elemento na fila \n";
    cout << "\n 6 \t- Ver fim da fila\n";
    cout << "\n 7 \t- Ver inicio da fila\n";
    cout << "\n 8 \t- Adicionar elemento sem prioridade\n";
    cout << "\n 9 \t- Preenche aleatório\n";
    cout << "\n 10 \t- Esvaziar fila\n";

    cin >> op;
    return op;
}