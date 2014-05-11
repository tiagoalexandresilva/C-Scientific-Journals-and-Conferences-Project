#ifndef MODULOF_H
#define MODULOF_H

typedef struct sLNaut{
	int naut; 
	int nart;
	struct sLNaut * next;
}*LNaut, NodoLNA;


typedef struct sAVL{
	int ano;
	LNaut lnaut;
	struct sAVL * esq, *dir;
}*AVL, NodoAVL;


void criaEstCompl(LNaut lna, AVL raiz);
LNaut criaLNaut();
void inserirAut(LNaut lna, int nraut);
AVL adicionarAVL(AVL *raiz, int year, int nraut);
void eliminarLNaut(LNaut lna);
void eliminarAVL(AVL raiz);
int contautores(char * aut);


#endif
