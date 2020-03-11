#include <stdio.h>
#include <stdlib.h>
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

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}
/**
//precisa de mais atenção
void buscaBinariaRecursivo(Endereco *e, char cp[8],long min, long max, int vezes){
    long meio = indices / 2;
    vezes++;
    if(strncmp(e[meio].cep, cp, 8) == 0){
        printf("%i", vezes);
        printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e[indices].logradouro,e[indices].bairro,e[indices].cidade,e[indices].uf,e[indices].sigla,e[indices].cep);
    }

    else if(strncmp(e[meio].cep, cp, 8) > 0){
        printf("Ta atrás");
        buscaBinaria(e, cp, meio - 1, vezes);
    }


    else{
        printf("Tá na frente");
        buscaBinaria(e, cp, meio + 1,vezes);
    }
}
**/

void buscaBinaria(FILE *f, long min, long max, char cp[8]){
    long meio;
    long offset;
    int i = 0;
    Endereco e;

    while (min <= max){
        i++;
        meio = (min + max)/ 2;
        offset = meio * sizeof(Endereco);
        fseek(f, offset, SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);

        if(strncmp(e.cep, cp, 8) == 0){
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            break;
        }
        
        else if(strncmp(e.cep, cp, 8) > 0){
            max = meio + 1;

        }
        else{
            min = meio - 1;

        }

    }

    
    printf("%i vezes\n", i);


    
}

int main(int argc, char**argv){

    FILE *f;

    
    if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}
    
    /**
    //Ordenar
    f = fopen("cep.dat", "r");

    fseek(f, 0, SEEK_END);
    long tamanhoArquivo = ftell(f);
    long numero_indices = tamanhoArquivo / sizeof(Endereco);

    rewind(f);
    Endereco *e;
    e = (Endereco*) malloc(tamanhoArquivo);
    fread(e, sizeof(Endereco), numero_indices, f);
    fclose(f);

    qsort(e,numero_indices,sizeof(Endereco),compara);

    f = fopen("cep_ordenado.dat", "w");
    fwrite(e, sizeof(Endereco), numero_indices, f);
    fclose(f);
    free(e);
    **/
    
   
    f = fopen("cep_ordenado.dat", "r");
    fseek(f, 1, SEEK_END);
    long tamanhoArquivo = ftell(f);
    rewind(f);

    buscaBinaria(f, 0, tamanhoArquivo/sizeof(Endereco), argv[1]);

}