#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "moduloF.h" 

#define max(x,y)    (((x) > (y)) ? (x) : (y))




AVL encontraNodo(AVL raiz, int year)
{ 
	AVL aux = raiz;
	
	while(aux != NULL && aux->ano != year)
	{
		if(year < aux->ano)
			aux = aux -> esq;
		else
			aux = aux -> dir;
    }
    return aux; 
} 


int calculaTotalArt(LNaut lna)
{
	int res = 0;
	while(lna != NULL)
	{
			res += lna->nart;
			lna = lna->next;
	}
	return res;
}


LNaut criaLNaut(){

	LNaut novo;
	novo = (LNaut)malloc(sizeof(NodoLNA));
	novo->next = NULL;
	return novo;
}


int procuraNaut(LNaut lna, int naut){

	LNaut aux;
	aux = lna;
	while(aux != NULL && naut != aux->naut){
		aux=aux->next;
	}

	if(aux != NULL)
		return 1;
	else
		return 0;
}


void inserirAut(LNaut lna, int nraut){
	
	LNaut ant = lna;
	LNaut aux = lna->next;
	LNaut novo;


	if(procuraNaut(lna, nraut)){
		while( aux->naut != nraut){
			aux = aux -> next;
		}
		aux -> nart++;
	}
	else{
		while(aux!= NULL && aux -> naut < nraut){
			ant=aux;
			aux=aux->next;
		}

		novo = (LNaut)malloc(sizeof(NodoLNA));
		novo->naut = nraut;
		novo->next = aux;
		novo->nart = 1;
		ant->next = novo;
	}
}


void imprimirLNaut(LNaut lna, FILE * file){
	LNaut aux = lna->next;
	if(aux != NULL){
		while(aux!= NULL){
			printf("%d,%d\n", aux->naut,aux->nart);
			fprintf(file, "\"%d\",\"%d\"\n", aux->naut,aux->nart);
		    aux=aux->next;
		}
	}
}


void eliminarLNaut(LNaut lna)
{
	if(lna){
		eliminarLNaut(lna->next);
		free(lna);
	}
}


int contautores(char * aut){
	int cont = 0;
	while(*aut != '\0'){
		if(*aut == ',')
			cont++;
		
		aut++;
	}
	return cont+1;
}


AVL rotacaoLL(AVL pai) 
{ 
    AVL filho = pai->esq; 
    pai->esq = filho->dir; 
    filho->dir = pai; 
    return filho; 
} 


AVL rotacaoRR(AVL pai) 
{ 
    AVL filho = pai->dir; 
    pai->dir = filho->esq; 
    filho->esq = pai; 
    return filho; 
} 


AVL rotacaoRL(AVL pai) 
{ 
    AVL filho = pai->dir; 
    pai->dir = rotacaoLL(filho); 
    return rotacaoRR(pai); 
} 


AVL rotacaoLR(AVL pai) 
{ 
    AVL filho = pai->esq; 
    pai->esq = rotacaoRR(filho); 
    return rotacaoLL(pai);
} 

int alturaAVL(AVL nodo)
{ 
    int altura=0; 
    if(nodo != NULL) 
        altura = 1+max(alturaAVL(nodo->esq),alturaAVL(nodo->dir)); 
    return altura;
} 

 
int factorbalanceamento(AVL nodo)
{ 
    if(nodo == NULL) return 0; 
    return alturaAVL(nodo->esq) - alturaAVL(nodo->dir); 
} 

 
AVL balancearAVL(AVL *nodo)
{ 
    int dif_altura = factorbalanceamento(*nodo); 
     
    if(dif_altura > 1) 
	{ 
        if(factorbalanceamento((*nodo)->esq) > 0) 
            *nodo = rotacaoLL(*nodo); 
        else  
            *nodo = rotacaoLR(*nodo); 
    } 
    else if(dif_altura < -1)
{ 
        if(factorbalanceamento((*nodo)->dir) < 0) 
            *nodo = rotacaoRR(*nodo); 
        else 
            *nodo = rotacaoRL(*nodo); 
    } 
    return *nodo; 
} 


AVL adicionarAVL(AVL *raiz, int year, int nraut)
{ 
    if(*raiz == NULL) 
    { 
        *raiz = (AVL)malloc(sizeof(NodoAVL));
        (*raiz)->ano = year;
        (*raiz)->lnaut = criaLNaut();
         inserirAut((*raiz)->lnaut, nraut);   
        (*raiz)->esq = (*raiz)->dir = NULL;
    } 
    else if(year < (*raiz)->ano) 
    { 
        (*raiz)->esq = adicionarAVL(&((*raiz)->esq), year, nraut); 
        (*raiz) = balancearAVL(raiz);  
    } 
    else if(year > (*raiz)->ano) 
    { 
        (*raiz)->dir = adicionarAVL(&((*raiz)->dir), year, nraut); 
        (*raiz) = balancearAVL(raiz); 
    } 
    else if(year == (*raiz)->ano)
    { 
        inserirAut((*raiz)->lnaut, nraut);
    } 
    return *raiz; 
} 


void imprimirLNautAVL(FILE * file, LNaut lna, int year)
{
	LNaut aux = lna->next;
	if(aux != NULL){
		while(aux!= NULL){
			printf("%d,%d,%d\n", year, aux->naut, aux->nart);
			fprintf(file, "\"%d\",\"%d\",\"%d\"\n", year, aux->naut, aux->nart);
		    aux=aux->next;
		}
	}
}


void imprimirAVL(AVL nodo, FILE * file)
{
	AVL aux = nodo;
	if(aux == NULL)
        return;
    else{
		imprimirAVL(aux -> esq, file);
		imprimirLNautAVL(file, aux->lnaut, aux->ano); 
        imprimirAVL(aux -> dir, file);
    }
}

void eliminarAVL(AVL raiz)
{
	if(raiz)
	{
		eliminarAVL(raiz->esq);
		eliminarAVL(raiz->dir);
		eliminarLNaut(raiz->lnaut);
		free(raiz);
	}
}


void trata_datas3(FILE * file, AVL raiz){
	FILE * file2;
	char str[50], *p = NULL;
	int total = 0, inicio, final;
	AVL aux = NULL;
	LNaut aux2 = NULL;
	
	file2 = fopen("datas3.txt","r");
	
	if(file2)
	{
		while(!feof(file2))
		{
			fgets(str, 50, file2);
	  		if(!feof(file))
	    	{	
				p = strchr(str,'\n');
				if(p) {*p = '\0';}
				
				sscanf(str, "%d-%d", &inicio, &final);
				
				while(inicio <= final){
					aux = encontraNodo(raiz, inicio);
					if(aux != NULL){
						aux2 = aux->lnaut->next;
						total += calculaTotalArt(aux2);
					}
					inicio++;
				}
				printf("%s,%d\n",str, total);
				fprintf(file,"\"%s\",\"%d\"\n",str, total);
				total = 0;
	    	}
		}
		fclose(file2);
	}
	else
	{
		printf("Não existe o ficheiro datas3.txt!\n");
	}
}


void trata_datas4(FILE * file, AVL raiz){
	FILE * file2;
	char str[50], *p = NULL;
	int total;
	AVL aux = NULL;
	LNaut aux2 = NULL;
	
	file2 = fopen("datas4.txt","r");
	
	if(file2)
	{
		fgets(str, 50, file2);
			
		p = strchr(str,'\n');
		if(p) {*p = '\0';}
				
		aux = encontraNodo(raiz, atoi(str));
		aux2 = aux->lnaut->next;
		total = calculaTotalArt(aux2);
		
		while(aux2 != NULL){
			printf("%s,%d,%.2f\n",str, aux2->naut, (double)((aux2->nart)*100)/total);
			fprintf(file,"\"%s\",\"%d\",\"%.2f\"\n",str, aux2->naut, (double)((aux2->nart)*100)/total);
			aux2 = aux2->next;
		}
		
		fclose(file2);
	}
	else
	{
		printf("Não existe o ficheiro datas4.txt!\n");
	}
}


void criaEstCompl(LNaut lna, AVL raiz)
{
	FILE * file;
	
	file = fopen("G.csv","w");
	 											
	fprintf(file,"\"ano\",\"#autores\",\"#artigos\" \n");
	printf("ano,#autores,#artigos\n");
	imprimirAVL(raiz, file);
	
	fprintf(file,"\"#autores\",\"#artigos\" \n");
	printf("#autores,#artigos\n");
	imprimirLNaut(lna,file);
	
	fprintf(file,"\"intervalo\",\"#artigos\" \n");
	printf("intervalo,#artigos\n");
	trata_datas3(file, raiz);
	
	fprintf(file,"\"ano\",\"#autores\",\"percentagem\" \n");
	printf("ano,#autores,percentagem\n");
	trata_datas4(file, raiz);
	
	fclose(file);
}
