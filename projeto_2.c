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

typedef struct a { /* Celula de uma lista de arestas */
	int nome;
	struct a *prox;
} Aresta;

typedef struct v {
	int nome;
	int distancia; /* Campo utilizado para marcar a distancia, no algoritmo BFS, conforme algoritmo do Cormen */
	int cor; /* Utilizamos cores para facilitar a manipulacao, sendo 0 para BRANCO, 1 para CINZA e 2 para PRETO */
	int d, f; /* Valores para o calculo de tempo no algoritmo DFS, conforme algoritmo do Cormen */
	int pi; /* Representa o predecessor do vertice na busca, indicando o indice em G */
	Aresta *prim;
} Vertice;

typedef struct f { /* Struct da fila para o algoritmo BFS */
	int tamanhoFila;
	Vertice fila[100];
} Fila;

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
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem) {
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d", ordem);
	printf("\nLista de Adjacencia:\n");

	for (i = 0; i < ordem; i++){
		printf("\n    V%d: ", i);
		aux = G[i].prim;
		for(; aux != NULL; aux = aux->prox)
			printf(" V%d", aux->nome);
	}
	printf("\n\n");
}

char imprimeCor(int codigoCor) {
	if (codigoCor == 0)
		printf("BRANCO");
	else if (codigoCor == 1)
		printf("CINZA");
	else
		printf("PRETO");
}

void imprimeResultadoBFS(Vertice G[], int ordem) {
    int i;

	printf("\nResultado Busca em Largura (BFS):\n");

	for (i = 0; i < ordem; i++){
		printf("\nV%d: ", i);
        printf("\n\t- COR: "); imprimeCor(G[i].cor);
        if (G[i].pi == -1) { /* Raiz */
            printf("\n\t- ANTECESSOR (pi): nao ha, e a raiz");
        } else {
            printf("\n\t- ANTECESSOR (pi): V%d", G[i].pi);
        }
        printf("\n\t- DISTANCIA DA RAIZ (s): %d", G[i].distancia);
	}
	printf("\n\n");
}

void imprimeResultadoDFS(Vertice G[], int ordem) {
    int i;

	printf("\nResultado Busca em Profundidade (DFS):\n");

	for (i = 0; i < ordem; i++){
		printf("\nV%d: ", i);
        printf("\n\t- COR: "); imprimeCor(G[i].cor);
        if (G[i].pi == -1) { /* Raiz */
            printf("\n\t- ANTECESSOR (pi): nao ha, e a raiz");
        } else {
            printf("\n\t- ANTECESSOR (pi): V%d", G[i].pi);
        }
        printf("\n\t- TEMPO DE DESCOBRIMENTO (d): %d", G[i].d);
        printf("\n\t- TEMPO DE FINALIZACAO (f): %d", G[i].f);
	}
	printf("\n\n");
}

int DFS_visit(Vertice G[], int indice, int tempo) {
	int i;

	tempo = tempo + 1;
	G[indice].d = tempo;
	G[indice].cor = CINZA;

	Aresta *aux = G[indice].prim;

	while (aux != NULL) { /* para cada vertice em adjacencia a G[indice] */
		int v = aux->nome;
		if (G[v].cor == BRANCO) {
			G[v].pi = indice;
			tempo = DFS_visit(G, v, tempo);
		}
		aux = aux->prox;
	}

	G[indice].cor = PRETO;
	tempo = tempo + 1;
	G[indice].f = tempo;

	return tempo;
}

void DFS(Vertice G[], int ordem) {
	int tempo, i;
	/* Iniciando todos os vertices com cor BRANCO e antecessor NULO */
	for (i = 0; i < ordem; i++) {
		G[i].cor = BRANCO;
		G[i].pi = -1;
	}

	tempo = 0; /* Inicializando tempo */

	for (i = 0; i < ordem; i++) {
		if (G[i].cor == BRANCO)
			DFS_visit(G, i, tempo);
	}
}

void enfileira(Fila *f, Vertice v) {
    f->fila[f->tamanhoFila] = v;
    f->tamanhoFila++;
}

Vertice desenfileira(Fila *f) {
    int i;
    Vertice v = f->fila[0];
    f->tamanhoFila--;
    for (i = 0; i < f->tamanhoFila; i++) {
        f->fila[i] = f->fila[i+1];
    }

    return v;
}

void BFS(Vertice G[], int ordem, int s) {
	int i;

	for (i = 0; i < ordem; i++) {
		if (i != s) { /* iteramos sobre os vertices de G menos, a raiz a arvore de largura */
			G[i].cor = BRANCO;
			G[i].distancia = -1; /* utilizamos -1 para sinalizar infinito */
			G[i].pi = -1;
		}
	}

	G[s].cor = CINZA;
	G[s].distancia = 0;
	G[s].pi = -1;

	Fila f;  /* declaracao da fila */
	f.tamanhoFila = 0;
	enfileira(&f, G[s]);	/* coloca a raiz na fila */

	  while (f.tamanhoFila != 0) {
		Vertice u = desenfileira(&f);

		Aresta *a = u.prim;

		/* iteramos na lista de adjacencia de u */
  		while(a != NULL) {
  			Vertice v = G[a->nome];
 			if (v.cor == BRANCO) {
    			v.cor = CINZA;
    			v.distancia = u.distancia + 1;
    			v.pi = u.nome;
				enfileira(&f, v);	/* enfileira v */
			}
			a = a->prox;
		}
    	u.cor = PRETO;
    	G[u.nome] = u; /* atualizar G */
	}
}

/*
 * Programinha simples para testar a representacao de grafo
 */
int main(int argc, char *argv[]) {
	Vertice *G;
	int ordemG = 8;

	criaGrafo(&G, ordemG);

	acrescentaAresta(G, ordemG, 0, 1);
	acrescentaAresta(G, ordemG, 0, 4);
	acrescentaAresta(G, ordemG, 1, 5);
	acrescentaAresta(G, ordemG, 2, 3);
	acrescentaAresta(G, ordemG, 2, 6);
	acrescentaAresta(G, ordemG, 3, 6);
	acrescentaAresta(G, ordemG, 3, 7);
	acrescentaAresta(G, ordemG, 5, 2);
	acrescentaAresta(G, ordemG, 5, 6);
	acrescentaAresta(G, ordemG, 6, 7);

	imprimeGrafo(G, ordemG);

	DFS(G, ordemG);
    imprimeResultadoDFS(G, ordemG);

	BFS(G, ordemG, 0);
    imprimeResultadoBFS(G, ordemG);

	destroiGrafo(&G, ordemG);
	return 0;
}
