#include <stdio.h>
#include <algorithm>

#define TAMANHO 4096 //Quantos endereções vão estar no buffer
#define QTD_BLOCO 171 //Tam arquivo / tam_bloco (to sendo meio preguiçoso aqui definindo isso na mão)


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

#define TAM_BLOCO (TAMANHO * sizeof(Endereco))

struct _buffer
{   
    Endereco endArr[TAMANHO];
    int blocoId;
};

void ordena(FILE *f, buffer *buff, int qtd_bloco){
    
}


Endereco* leia(FILE *f, buffer *buff, long indice, int qtd_bloco){
    int indiceBloco = indice / TAMANHO;
    long indiceNoBloco = indice / qtd_bloco;

    //Se os ids batem, o bloco na memoria é o bloco que queremos acessar
    if(indiceBloco == buff->blocoId){
        return &buff->endArr[indiceNoBloco];
    }

    //Se não for o bloco que queremos acessar, verificamos se o bloco esta a frente do bloco na memoria ou atrás dele
    //Não precisava desse else if e o else em baixo, mas prefiro deixar claro o que ta rolando aqui 
      
    
    if (indiceBloco > buff->blocoId){
        //Como meu bloco na memoria esta antes do bloco que eu quero, eu só preciso andar com o carro a diferença de blocos
        int diff = indiceBloco - buff->blocoId;
        //Posso ao invez de dar varios pulos de tamanho fixo, dar um pulo só do offset abaixo
        long offset = diff * TAM_BLOCO;

        for(int i=0; i < diff; i++){
            fseek(f, TAM_BLOCO ,SEEK_CUR);
        }

        fread(buff->endArr, TAM_BLOCO, TAMANHO, f);

        return &buff->endArr[indiceNoBloco];
    }

    //Meu bloco esta atrás do atual na memória, então faço um rewind e leio de bloco em bloco

    rewind(f);

    for(int i = 0; i < indiceBloco; i++){
        fseek(f, TAM_BLOCO, SEEK_CUR);
    }

    fread(buff->endArr, TAM_BLOCO, TAMANHO, f);

    return &buff->endArr[indiceNoBloco];
}

void escreva(FILE *f, buffer *buff, long indice, int qtd_bloco, Endereco *e){
    int indiceBloco = indice / TAMANHO;
    long indiceNoBloco = indice / qtd_bloco;

    if(indiceBloco == buff->blocoId){
        buff->endArr[indiceNoBloco] = e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
    }

    else if(indiceBloco > buff->blocoId){
        int diff = indiceBloco - buff->blocoId;
        long offset = diff * TAM_BLOCO;

        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = *e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
    }

    else{
        long offset = indiceBloco * TAM_BLOCO;

        rewind(f);
        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);

    }


}


int main(int argc, char** argv){
    
    FILE *f;
    buffer b;
    long tamanhoArquivo;
    int qtd_bloco;

    f = fopen("C:\\Users\\Zirondi\\Desktop\\cep.dat", "r");
    fseek(f,0,SEEK_END);
    tamanhoArquivo = ftell(f);
    qtd_bloco = tamanhoArquivo / TAM_BLOCO;
    rewind(f);

    long ind = 500000;
    Endereco *e = leia(f, &b, ind, qtd_bloco);

    printf(e->cep);





    /**
    printf("%s", argv[0]);

    //Tratamento de erro
    if(argc != 2){
        fprintf(stderr, "USO: %s [CEP]", argv[0]);
        return 1;
    }

    ordena(f);
    **/


}