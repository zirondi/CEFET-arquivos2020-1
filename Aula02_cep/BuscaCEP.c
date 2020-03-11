#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

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

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;

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
	while(qt > 0)
	{
		c++;
		//strncmp vs strcmp (primeiro eu nao tenho quebra de string e preciso explicitar(8), o segundo eu tenho)
		//retorna 0, pos ou neg (comparação de strings por subtração)
		if(strncmp(argv[1],e.cep,8)==0)
		{
			//.72 limitando o numero de chars que sera impresso
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			break;
		}
		qt = fread(&e,sizeof(Endereco),1,f);		
	}
	printf("Total Lido: %d\n", c);
	fclose(f);
}

