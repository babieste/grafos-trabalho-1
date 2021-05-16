/*
	Barbara E. Fernandez - 31937039
	Salomon A. Motoryn - 41825128
*/

/* 
 * REPRESENTACAO DE GRAFOS - Versao 2021/1
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

#define BRANCO 0
#define CINZA 1
#define PRETO 2

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a { /* Celula de uma lista de arestas */
	int nome;
	struct a *prox;
} Aresta;

typedef struct v {
	int nome;
	int marca;
	int cor; /* Utilizamos cores para facilitar a manipulacao, sendo 0 para BRANCO, 1 para CINZA e 2 para PRETO */
	int d, f; /* Valores para o calculo de tempo no algoritmo DFS, conforme algoritmo do Cormen */
	Aresta *pi; /* Representa o predecessor do vertice na busca */
	Aresta *prim;
} Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2);
int calculaTamanho(Vertice G[], int ordem);
void imprimeGrafo(Vertice G[], int ordem);

/*
 * Implementacao das funcoes para manipulacao de grafos 
 */
 
/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vertice **G, int ordem) {
	int i;
	*G = (Vertice*) malloc(sizeof(Vertice) * ordem); /* Alocacao dinamica de um vetor de vertices */
	
	for (i = 0; i < ordem; i++) {
		(*G)[i].nome = i;
		(*G)[i].marca = 0; /* representa nao manipulado */
		(*G)[i].prim = NULL;    /* Cada vertice sem nenhuma aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem) {
	int i;
    Aresta *a, *n;
    
	for (i = 0; i < ordem; i++) { /* Remove lista de adjacencia de cada vertice */
	    a = (*G)[i].prim;
        while (a != NULL) {
              n = a->prox;
              free(a);
              a = n;
        }
	}
    free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2) {
    Aresta *A1, *A2;
    
	if (v1 < 0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2 < 0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1 = (Aresta *) malloc(sizeof(Aresta));
	A1->nome = v2;
	A1->prox = G[v1].prim;
	G[v1].prim = A1;

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */
	if (v1 == v2) return 1;

	A2 = (Aresta *) malloc(sizeof(Aresta));
	A2->nome = v1;
	A2->prox = G[v2].prim;
	G[v2].prim = A2;
	
	return 1;
}

/*  
 * Funcao que retorna o tamanho de um grafo
 */
int  calculaTamanho(Vertice G[], int ordem) {
	int i;
	int totalArestas = 0;
	
	for (i = 0; i < ordem; i++){
		int j;
		Aresta *aux = G[i].prim;
		for(j = 0; aux != NULL; aux = aux->prox, j++)
            if (aux->nome == i) j++; /* laco "conta em dobro" */
		totalArestas += j;	
	}
	return totalArestas/2 + ordem;
}

/*
	Um grafo conexo G e uma arvore se e somente se |AG| = |VG|-1.
	Se a condicao e verdadeira, retorna 1. Caso contrario, retorna 0.
*/
int verificaCondicaoArvore(Vertice G[], int ordem) {
	int totalArestas = calculaTamanho(G, ordem) - ordem;
	if (totalArestas == (ordem - 1)) return 1;
	return 0;
}

/*
	Como utilizamos o campo "marca" para verificarmos tanto a conexidade
	quanto se o grafo possui lacos, a cada verificacao precisamos "resetar"
	esse campo para que nao haja interferencia, isto e, para todo vertice,
	marcamos o campo como zero.
*/
void resetaGrafo(Vertice G[], int ordem) {
	int i = 0;

	while (i < ordem) {
		Aresta *aux = G[i].prim;

		while (aux != NULL) {
			int j = aux->nome;
			G[j].marca = 0;
			aux = aux->prox;
		}

		i++;
	}
}

/*
	Um grafo possui lacos se um vertice possui adjacencia a ele mesmo.
	Caso o grafo em questao possua ao menos um laco, ele nao podera
	ser arvore. A funcao retorna 1 para verdadeiro e 0 para falso.
*/
int possuiLaco(Vertice G[], int ordem) {
	int possuiLaco = 1;
	int i = 0;

	while (i < ordem) {
		Aresta *aux = G[i].prim;

		if (aux == NULL)
			G[i].marca = 1; /* se o vertice nao possui arestas entao nao possui lacos */

		while (aux != NULL) {
			int j = aux->nome;

			if(G[j].nome == i)
				G[i].marca = 1;

			aux = aux->prox;
		}

		i++;
	}

	int j;
	for (j = 0; j < ordem; j++) {
		if (G[j].marca == 0)
			possuiLaco = 0;
	}

	return possuiLaco;
}

/*
	Verifica se o grafo e conexo, retornando 1
	em caso positivo e 0 em caso negativo.
*/
int verificaConexidade(Vertice G[], int ordem) {
	int eConexo = 1;
	int i = 0;
	/* Escolha um vertice v0 de G e marque-o  com 1 */
	G[i].marca = 1;
	
	/*
		Se existir uma aresta que incide em um vertice v[i] marcado com 1
		e um vertice v[j] marcado com 0, marque v[j] com 1 ate que nenhum
		novo vertice seja marcado com 1
	*/
	while (i < ordem) {
		Aresta *aux = G[i].prim; /* referencio a primeira aresta da lista */

		if (aux == NULL) {
			G[i].marca = 0; /* se o vertice nao possui arestas entao e imediatamente desconexo */
		}
		
		while (aux != NULL) {
			int j = aux->nome; /* pego o nome do vertice adjacente */

			if (G[j].marca == 0)
				G[j].marca = 1;
			
			aux = aux->prox;
		}
		
		i++;
	}
	
	/*
		itero sobre os vertices para verificar se ha vertices que
		nao foram marcados. Se ha, o grafo nao e conexo.
	*/
	int j;
	for(j = 0; j < ordem; j++) {
		if (G[j].marca == 0)
			eConexo = 0;
	}
	
	return eConexo;
}

/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem) {
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d", ordem);
	printf("\nLista de Adjacencia:\n");

	for (i = 0; i < ordem; i++){
		printf("\n    V%d (Marca:%d): ", i, G[i].marca);
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox)
			printf(" V%d", aux->nome);
	}
	printf("\n\n");
}

int DFS_visit(Vertice G[], int indice, int tempo) {
	int i;

	tempo = tempo + 1;
	G[indice].d = tempo;
	G[indice].cor = CINZA;
	
	printf("\n\nVisitando vertice V%d\n\t- cor: %d\n\t- descoberto (d): %d", indice, G[indice].cor, G[indice].d);

	Aresta *aux = G[indice].prim;

	while (aux != NULL) { /* para cada vertice em adjacencia a G[indice] */
		int v = aux->nome;
		if (G[v].cor == BRANCO) {
			G[v].pi = aux;
			tempo = DFS_visit(G, v, tempo);
		}
		aux = aux->prox;
	}

	G[indice].cor = PRETO;
	tempo = tempo + 1;
	G[indice].f = tempo;
	
	printf("\n\nRetornando ao vertice V%d\n\t- cor: %d\n\t- terminado (f): %d\n", indice, G[indice].cor, G[indice].f);
	return tempo;
}

void DFS(Vertice G[], int ordem) {
	int tempo, i;
	/* Iniciando todos os vertices com cor BRANCO e antecessor NULO */
	for (i = 0; i < ordem; i++) {
		G[i].cor = BRANCO;
		G[i].pi = NULL;
	}
	
	tempo = 0; /* Inicializando tempo */
	
	for (i = 0; i < ordem; i++) {
		if (G[i].cor == BRANCO)
			DFS_visit(G, i, tempo);
	}
}

void enfileira(Vertice fila[], Vertice v) {
    fila[tamanhoFila] = v;
    tamanhoFila++;
}

void desenfileira(Vertice fila[], Vertice v) {
    int i;
    v = fila[0];
    tamanhoFila--;
    for (i = 0; i < tamanhoFila-1; i++) {
        fila[i] = fila[i+1];
    }
}


void BFS(Vertice G[], int ordem, int s){

  
	for (i = 0; i < ordem; i++) - {s} {
		G[i].cor = BRANCO;	
		G[i].d = -1; 
		G[i].pi = NULL;
	}
	s.cor = CINZA;
	s.d = 0;
	s.pi = NULL;
	
	Vertice fila[100];  //representar uma fila
	int tamanhofila = 0
	enfileira(G[], s);	//colocar na fila
  	while (fila != 0){
    		u = desenfileira(G[], s);		//tirar da fila
  		while(v != NULL){    
 			if (G[v].cor==BRANCO){
        			G[v].cor=CINZA;
        			G[v].d=u.d+1;
        			G[v].pi=u;
				enfileira(G[], s);	//colocar na fila
			}
		}
    		G[u].cor = PRETO;
	}
}

/*
 * Programinha simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
	Vertice *G;
	int ordemG = 6;
	int eConexo = 0;
	int condicao = 0;
	int eArvore = 0;
	int temLaco = 0;
		
	criaGrafo(&G, ordemG);


	acrescentaAresta(G, ordemG, 0, 0);
	acrescentaAresta(G, ordemG, 3, 4);
	acrescentaAresta(G, ordemG, 4, 3);
	acrescentaAresta(G, ordemG, 4, 2);
	acrescentaAresta(G, ordemG, 5, 4);
	acrescentaAresta(G, ordemG, 2, 3);
	acrescentaAresta(G, ordemG, 3, 0);

	
	/*
	acrescentaAresta(G, ordemG, 0, 1);
	acrescentaAresta(G, ordemG, 1, 2);
	acrescentaAresta(G, ordemG, 2, 5);
	acrescentaAresta(G, ordemG, 5, 4);
	acrescentaAresta(G, ordemG, 1, 3);
	*/
	
	imprimeGrafo(G, ordemG);
	
	DFS(G, ordemG);

	destroiGrafo(&G, ordemG);
    system("PAUSE");
	return 0;
}



