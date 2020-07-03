/* Diego Paredes Canellas 1821502 3WA*/
/* Eduardo César Accioly Colnago 1821187 3WB */


#include "conv_utf.h"
#include <stdio.h>
#include <stdlib.h>


int utf32_8(FILE *arq_entrada, FILE *arq_saida){
	unsigned char *array_32_8;
	unsigned char BOM_l[4]={0xff,0xfe,0x00,0x00};
	unsigned char BOM_b[4]={0x00,0x00,0xfe,0xff};
	unsigned char BOM[4];
	int size = 0;
	int i;
	fseek(arq_entrada, 0, SEEK_END); //ponteiro aponta para o final do arq
	size = ftell(arq_entrada); //retorna valor da posicao do ponteiro
	fseek(arq_entrada, 0, SEEK_SET); //ponteiro volta para o inicio
	array_32_8 = malloc(size);
	if (arq_entrada == NULL)
	{
		printf("arquivo ruim");
		return -1;
	}
	for (i=0;i<4;i++){
		BOM[i] = fgetc(arq_entrada);
	}
	for (i=0;i<4;i++){
		if (BOM[i] != BOM_l[i]){
			if(BOM[i] != BOM_b[i]){
				printf("BOM inválido");
				return -1;
			}
		}
	}
	fseek(arq_entrada, 0, SEEK_SET); //ponteiro volta para inicio apos conferir BOM	
	cria_array(arq_entrada, array_32_8);
	func32_8(array_32_8,size,arq_saida);
	free(array_32_8);
	return 0;
}


void func32_8(unsigned char *c,int size,FILE *arq_saida)
{
    int i=4;
	unsigned char *utf32_vet = c;
	unsigned char unicode3b[3];
    unsigned char unicode2b[2];
    while(i<=size-1)
	{
        if(utf32_vet[i+2]&0x01)
		{
            unicode3b[0]=utf32_vet[i+2];
            unicode3b[1]=utf32_vet[i+1];
            unicode3b[2]=utf32_vet[i];
			unicode_utf8_4byte(unicode3b, arq_saida);
        }
		else
		{
        	unicode2b[1]=utf32_vet[i]; 
        	unicode2b[0]=utf32_vet[i+1];
       		if(unicode2b[0]>=0x08&&unicode2b[0]<=0xFF)
			{
        		unicode_utf8_3byte(unicode2b, arq_saida);
        	}
        	else if(unicode2b[1]<=0x7F&&unicode2b[0]==0x00)
			{
				fprintf(arq_saida,"%c",unicode2b[1]);
        		
        	}
			else
			{
				unicode_utf8_2byte(unicode2b,arq_saida);
       		}
		}
		i+=4;
	}
}


void unicode_utf8_3byte(unsigned char* c, FILE *arq_saida){
	unsigned char utf8[3]={0x00,0x00,0x00};
	unsigned char aux=c[1]&0x3F;//ultimo caracter
	unsigned char aux3;
	unsigned char aux2;
	aux=aux|0x80;
	c[1]=c[1]>>6;//segundo caracter
	aux2=c[0]<<2;
	aux2=aux2|c[1]; 
	aux2=aux2|0x80; 
	aux2=aux2&0xBF;
	c[0]=c[0]>>4;//primeiro caracter
	aux3=c[0]|0xE0;
	utf8[0]=aux3;
	utf8[1]=aux2; 
	utf8[2]=aux;
	reescreve_arq(arq_saida, utf8, 3);
	return;
}

void unicode_utf8_2byte(unsigned char* c, FILE *arq_saida){
	unsigned char utf8[2]={0x00,0x00};
	unsigned char aux=c[1]&0X3F;
	unsigned char a=aux|0x80;
	unsigned char aux2;
	unsigned char b=c[1]&0xc0; //pega os 2 bits que aux "jogou fora" e prepara para juntar no 2 byte
	b=b>>6;
	c[0]=c[0]<<2;
	aux2=c[0]|b;
	aux2=aux2|0xc0;
	utf8[0]=aux2;
	utf8[1]=a;
	reescreve_arq(arq_saida, utf8, 2);
	return;
}

void unicode_utf8_4byte(unsigned char* c, FILE *arq_saida){
	unsigned char aux2;
	unsigned char y;
	unsigned char aux4;
	unsigned char aux3;
	unsigned char utf8[4]={0x00,0x00,0x00,0x00}; 
	unsigned char aux=c[2]&0x3F;//ultimo caracter
	aux=aux|0x80;
	c[2]=c[2]>>6; //terceiro caracter
	aux2=c[1]<<2;
	aux2=aux2|c[2];
	aux2=aux2|0x80;
	aux2=aux2&0xBF; 
	y=c[1];
	y=y>>4; //segundo caracter
	aux3=c[0]<<4;
	aux3=aux3|y;
	aux3=aux3|0x80;
	aux3=aux3&0xBF;
	aux4=c[0]>>1;  //primeiro caracter
	aux4=aux4|0xF0;
	utf8[0]=aux4;
	utf8[1]=aux3;
	utf8[2]=aux2;
	utf8[3]=aux;
	reescreve_arq(arq_saida, utf8, 4);
	return;
}

int utf8_32(FILE *arq_entrada, FILE *arq_saida)
{
	unsigned char *v;
	unsigned char inicio[4]={0xff,0xfe,0x00,0x00};
	int size = 0;
	fseek(arq_entrada, 0, SEEK_END); //ponteiro aponta para o final do arq
	size = ftell(arq_entrada); //retorna valor da posicao do ponteiro
	fseek(arq_entrada, 0, SEEK_SET); //ponteiro volta para o inicio
	v = malloc(size);
	if (arq_entrada == NULL)
	{
		printf("Erro na leitura do arquivo");
		return -1;
	}
	cria_array(arq_entrada, v);
	fwrite(inicio ,1,4, arq_saida);
	utf8StrLen(v, size,arq_saida);
	if (arq_saida == NULL)
	{
		printf("Erro na gravação do arquivo");
		return -1;
	}
	fclose(arq_entrada);
	fclose(arq_saida);
	free(v);
	return 0;
}


void cria_array(FILE *arq_entrada, unsigned char *v)
{
	int i = 0;
	while (!feof(arq_entrada))
	{
		v[i] = fgetc(arq_entrada);
		i++;
	}
	return;
}

void utf8StrLen(unsigned char * Utf8Stream, int n, FILE *arq_saida) {
	int i = 0;
	int cont = 0;
	int qnt = 0;
	unsigned char *y;
	while (i < n) {
		unsigned char aux[4] = { 0x00,0x00,0x00,0x00 };
		if ((Utf8Stream[i] & 0xf0) == 0xf0) {
			qnt = 4;
			y = utf8_unicode(Utf8Stream, i, qnt);
			aux[0] = y[2];
			aux[1] = y[1];
			aux[2] = y[0];
			i += 4;
		}
		else if ((Utf8Stream[i] & 0xe0) == 0xe0) {
			qnt = 3;
			y = utf8_unicode(Utf8Stream, i, qnt);
			aux[0] = y[1];
			aux[1] = y[0];
			i += 3;
		}
		else if ((Utf8Stream[i] & 0xc0) == 0xc0) {
			qnt = 2;
			y = utf8_unicode(Utf8Stream, i, qnt);
			aux[0] = y[1];
			aux[1] = y[0];
			i += 2;
		}
		else {
			aux[0]=Utf8Stream[i];
			i++;
		}
		cont += 4;
		reescreve_arq(arq_saida, aux, 4);
	}
	return;
}

unsigned char *utf8_unicode(unsigned char *Utf8Stream, int indice, int bytes)
{
	unsigned char *test;
	unsigned char a, b, d;
	unsigned char aux;
	unsigned char aux2;
	unsigned char *c = malloc(bytes);
	int i;
	int n = indice;
	for (i=0; indice < n+bytes; indice++,i++)
	{
		c[i] = Utf8Stream[indice];
	}
	if (bytes == 2) {
		test = malloc(bytes);
		
		test[0] = 0x00;
		test[1] = 0x00;
		c[0] = c[0] & 0x3f;
		aux = c[0] >> 2;

		c[0] = c[0] << 6;
		c[1] = c[1] & 0x3f;
		b = c[0] | c[1];

		test[0] = aux;
		test[1] = b;
	}
	else if (bytes == 3) {
		test = malloc(bytes-1);
		test[0] = 0x00;
		test[1] = 0x00;
		c[1] = c[1] & 0x3F;
		c[0] = c[0] & 0x3F;
		aux2 = c[0] << 4;
		aux = c[1] << 6;
		c[2] = c[2] & 0x3F;
		c[1] = c[1] >> 2;
		a = c[2] | aux;
		b = c[1] | aux2;
		test[0] = b;
		test[1] = a;
	}
	else {
		test = malloc(bytes - 1);
		test[0] = 0x00;
		test[1] = 0x00;
		test[2] = 0x00;
		c[3] = c[3] & 0x3F;
		aux = c[2] << 6;
		a = c[3] | aux;
		c[2] = c[2] & 0x3F;
		c[2] = c[2] >> 2;
		aux2 = c[1] << 4;
		b = c[2] | aux2;
		c[1] = c[1] & 0x3F;
		c[1] = c[1] >> 4;
		c[0] = c[0] & 0x0F;
		d = c[0] | c[1];
		test[0] = d;
		test[1] = b;
		test[2] = a;
	}
	free(c);
	return test;
	}

int reescreve_arq(FILE *arq_saida, unsigned char *vetor_final, int n) {
	if(fwrite(vetor_final,1,n ,arq_saida)!=n){
	    printf("Erro na gravação do arquivo");
	    return -1;
	}
	return 1;
}
	

