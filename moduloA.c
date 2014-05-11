#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "moduloA.h"




char* strdup(char *str){
    int n = strlen(str) + 1;
    char *dup = malloc(n * sizeof(char));
    if(dup){
        strcpy(dup, str);
    }
    return dup;
}


char *strsep(char **stringp, const char *delim) {
	char *begin, *end;
	begin = *stringp;
	if(begin == NULL)
		return NULL;

	if(delim[0] == '\0' || delim[1] == '\0') {
		char ch = delim[0];
		if(ch == '\0')
			end = NULL;
		else {
			if(*begin == ch)
				end = begin;
			else if(*begin == '\0')
				end = NULL;
			else
				end = strchr(begin + 1, ch);
		}
	}
	else
		end = strpbrk(begin, delim);
	if(end) {
		*end++ = '\0';
		*stringp = end;
	}
	else
		*stringp = NULL;
	return begin;
}


int isnumeric(char *str)
{
  while(*str)
  {
    if(!isdigit(*str))
      return 0;
    str++;
  }

  return 1;
}


int getAnoConf(char *linha){
    int i=-1;
    char *aux = strdup(linha);
    while(aux && strstr(aux," ")){
        strsep(&aux," ");
    }
    if(isnumeric(aux) == 1)
		i = atoi(aux);
	
    return i;
}


void strTolower(char *s)
{
    for(; *s; s++){
        if(('A' <= *s) && (*s <= 'Z'))
            *s = 'a' + (*s - 'A');
	}
}


int testaTitulo(char * entrada)
{
	int res = 1;
	
	if(strlen(entrada) == 1)
		res=-1;
	else{
		strTolower(entrada);
		if((strstr(entrada, "preface") != NULL) || (strstr(entrada, "editorial") != NULL) ||(strstr(entrada, "errata") != NULL) || (strstr(entrada, "obituary") != NULL) || (strstr(entrada, "in memory of") != NULL) || (strstr(entrada, "isbn") != NULL))
			res = -1;
	}
	
	return res;
} 


int testaAutores(char * entrada)
{
	int res = 1;
	if(strlen(entrada) == 0)
		res = -1;
		
	return res;
}


int testaPag(char * entrada, int pagmin)		
{
	int res = 1, pgin, pgfin;
	if(sscanf(entrada, "%d-%d", &pgin, &pgfin) == 2 ){
		if(pagmin > ((pgfin-pgin)+1))
			res = -1;
	}
	else
		res = -1;
		
	return res;
}


int testaNrRev(char *token){
	int res = 1;
	char * p = NULL;
	
	p = strchr(token,')');
	if(p) {*p = '\0';}
	else{
		res = -1;
	}
	
	if(res == 1)
	{
		if(isnumeric(token) != 1)
			res = -1;
	}
	
	return res;
}


int testaVol(char *token){
	int res = 1;
	
	token++;
	if(isnumeric(token) != 1)
		res = -1;
	
	return res;
}


EstBasica inicializaEstBasica(){
	EstBasica aux;	
	aux=(EstBasica)malloc(sizeof(NodoEB));
	aux -> ent = 0;
	aux -> rej = 0;
	aux -> art = 0;
	aux -> rev = 0;
	aux -> conf = 0;
	
	return aux;
}


AVL analisaRev(EstBasica es, LNaut laut, AVL raiz, FILE * file2, char * nomef, int pagmin)
{	
	FILE * file;
	char str[1024];
	char * copia, * token;
	int cont = 0, nraut, ano;
	
	file = fopen(nomef,"r");
	
	if(file)
	{
		while(!feof(file))
		{
			fgets(str, 1024, file);
	  		if(!feof(file))
	    	{
				es->ent += 1;
				copia = strdup(str);
	
				token = strsep(&copia, " ");
				if(isnumeric(token) == 1 && strchr(copia, ':')!=NULL)
				{
					token = strsep(&copia, ":");
					if(testaAutores(token) == 1 && strchr(copia, '.')!=NULL)
					{
						nraut = contautores(token);
						token = strsep(&copia, ".");
						if(testaTitulo(token) == 1 && strchr(copia, '(')!=NULL && strchr(copia, ')')!=NULL)
						{
							token = strsep(&copia, "(");
							if(strchr(copia, '(')!=NULL && strchr(copia, ')')!=NULL)
							{
								token = strsep(&copia, ")");
								token = strsep(&copia, "(");
								if(testaVol(token)==1 && strchr(copia, ':')!=NULL)
								{
									token = strsep(&copia, ":");
									if(testaNrRev(token) == 1 && strchr(copia, ' ')!=NULL)
									{
										token = strsep(&copia, " ");
										if(testaPag(token, pagmin) == 1 && strchr(copia, '(')!=NULL && strchr(copia, ')')!=NULL)
										{
											token = strsep(&copia, "(");
											token = strsep(&copia, ")");
											if(isnumeric(token) == 1 && atoi(token) <= 2013 && atoi(token) >= 0  && strchr(copia, '\n')!=NULL)
											{ 
												ano = atoi(token);
												token = strsep(&copia, "\n");
												es -> art += 1;
												es -> rev += 1;
												inserirAut(laut,nraut);
												raiz = adicionarAVL(&raiz, ano, nraut);
											}
											else{
												es -> rej += 1;
												cont++;
											}
										}	
										else{
											es -> rej += 1;
											cont++;
										}
									}
									else{
										es -> rej += 1;
										cont++;
									}
								}
								else{
									es -> rej += 1;
									cont++;
								}
							}
							else{
								es -> rej += 1;
								cont++;
							}
						}
						else{
							es -> rej += 1;
							cont++;
						}
					}
					else{
						es -> rej += 1;
						cont++;
					}
				}
				else{
					es -> rej += 1;
					cont++;
				}
			}
		}
		fclose(file);
	}
	else
	{
		printf("Não existe o ficheiro %s!\n", nomef);
	}
	
	fprintf(file2,"%s %d\n", nomef, cont);
	return raiz;
}


AVL analisaConf(EstBasica es, LNaut laut, AVL raiz, FILE * file2, char * nomef, int pagmin)
{
	FILE * file;
	char str[1024];
	char * copia, * token;
	int cont = 0, nraut, ano;
	
	file = fopen(nomef,"r");
	
	if(file)
	{
		while(!feof(file))
		{
			fgets(str, 1024, file);
	  		if(!feof(file))
	    	{	
				es->ent += 1;
				copia = strdup(str);
	
				token = strsep(&copia, " ");
				if(copia != NULL && isnumeric(token) == 1)
				{
					token = strsep(&copia, ":");
					if(copia != NULL && testaAutores(token) == 1)
					{
						nraut = contautores(token);
						token = strsep(&copia, ".");
						if(copia != NULL && testaTitulo(token) == 1)
						{
							token = strsep(&copia, ":");
							if(copia != NULL)
							{
								ano = getAnoConf(token);
								
								if(ano <= 2013 && ano >= 0)
								{
									token = strsep(&copia, "\n");
									if(copia != NULL && testaPag(token, pagmin) == 1)
									{
										es -> art += 1;
										es -> conf += 1;
										inserirAut(laut,nraut);
										raiz = adicionarAVL(&raiz, ano, nraut);
									}
									else{
										es -> rej += 1;
										cont++;
									}
								}
								else{
									es -> rej += 1;
									cont++;
								}
							}
							else{
								es -> rej += 1;
								cont++;
							}
						}
						else{
							es -> rej += 1;
							cont++;
						}
					}
					else{
						es -> rej += 1;
						cont++;
					}
				}	
				else{
					es -> rej += 1;
					cont++;
				}
			}
		}
		fclose(file);
	}
	else
	{
		printf("Não existe o ficheiro %s!\n", nomef);
	}
	
	fprintf(file2,"%s %d\n", nomef, cont);
	return raiz;
}


void criaEstBasicatxt(EstBasica es)
{
	FILE * file;
	file = fopen("D.txt","w");
	fprintf(file,"Estatistica basica\n");
	fprintf(file,"------------------\n");
	fprintf(file,"%d entradas\n", es->ent);
	fprintf(file,"%d rejeitadas\n", es->rej);
	fprintf(file,"%d artigos\n", es->art);
	fprintf(file,"  %d em revista\n", es->rev);
	fprintf(file,"  %d em conferencia\n", es->conf);
	fclose(file);
}
