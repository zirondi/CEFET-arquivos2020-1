#include <stdio.h>
#include <string.h>

#define TAMANHO 4096

typedef struct _Endereco Endereco;
typedef struct _buffer buffer;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

struct _buffer
{   
    Endereco bloco[TAMANHO];
    int blocoId;
};

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;
	long posicao;
	int qtd, qtd1;
	buffer *b;

	b = 

	printf("%s", argv[0]);

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

	c = 0;
	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	f = fopen("cep.dat","r");
	qt = fread(&e,sizeof(Endereco),1,f);


	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd1 = posicao/sizeof(buffer);
	qtd = posicao/sizeof(b);

	printf("%s", qtd);

	fclose(f);
}
