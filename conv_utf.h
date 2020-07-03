/* Diego Paredes Canellas 1821502 3WA*/
/* Eduardo César Accioly Colnago 1821187 3WB */

#include <stdio.h>
#include <stdlib.h>

//Funçoes auxiliares utf8_32
int reescreve_arq(FILE *arq_saida, unsigned char *vetor_final, int n);
unsigned char *utf8_unicode(unsigned char *Utf8Stream, int indice, int bytes);
void utf8StrLen(unsigned char * Utf8Stream, int n, FILE *arq_saida);
void cria_array(FILE *arq_entrada, unsigned char *v);

//Funçoes auxiliares utf32_8
void unicode_utf8_4byte(unsigned char* c, FILE *arq_saida);
void unicode_utf8_2byte(unsigned char* c, FILE *arq_saida);
void unicode_utf8_3byte(unsigned char* c, FILE *arq_saida);
void func32_8(unsigned char *c,int size,FILE *arq_saida);


int utf8_32(FILE *arq_entrada, FILE *arq_saida);
int utf32_8(FILE *arq_entrada, FILE *arq_saida);
