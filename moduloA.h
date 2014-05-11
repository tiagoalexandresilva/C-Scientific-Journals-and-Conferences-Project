#ifndef MODULOA_H
#define MODULOA_H

#include "moduloF.h"

typedef struct sEstBasica{
	int ent;
	int rej;
	int art;
	int rev;
	int conf;
} *EstBasica, NodoEB;


void criaEstBasicatxt(EstBasica es);
EstBasica inicializaEstBasica();
AVL analisaConf(EstBasica es, LNaut laut, AVL raiz, FILE * file2, char * nomef, int pagmin);
AVL analisaRev(EstBasica es, LNaut laut, AVL raiz, FILE * file2, char * nomef, int pagmin);

#endif
