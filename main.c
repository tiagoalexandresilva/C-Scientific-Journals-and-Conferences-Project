#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "moduloA.h"
#include "moduloF.h"


int main()
{
	FILE * file, * file2;
	int pagmin = -1;
  	char str[200], * p = NULL;
  	EstBasica es = NULL;
  	LNaut laut = NULL;
  	AVL raiz = NULL;
  	
	file = fopen("lista.txt","r");
	
	if(file)
	{
		file2 = fopen("E.txt","w");
		fprintf(file2,"Lista Rejeitadas\n");
		fprintf(file2,"----------------\n");
		
		es = inicializaEstBasica();
		laut = criaLNaut();
		
		fgets(str, 200, file);
		sscanf(str, "%d", &pagmin);
		while(!feof(file))
		{
			fgets(str, 200, file);
	  		if(!feof(file))
	    	{
				p = strchr(str,'\n');
				if(p) {*p = '\0';}
				
				if(str[0] == 'c')
					raiz = analisaConf(es, laut, raiz, file2, str, pagmin);
				
				if(str[0] == 'j')
					raiz = analisaRev(es, laut, raiz, file2, str, pagmin);
	    	}
		}
		fclose(file2);
		fclose(file);
		
		criaEstBasicatxt(es);
		free(es);
		criaEstCompl(laut, raiz);
		eliminarLNaut(laut);
		eliminarAVL(raiz);
	}
	else
	{
		printf("Não existe o ficheiro lista.txt!\n");
	}
	return 1;
}

