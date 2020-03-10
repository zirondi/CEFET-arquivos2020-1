#include <stdio.h>
#include <iostream>
#include <algorithm>

#define TAMANHO 4096 //Quantos endereções vão estar no buffer



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
    Endereco endArr[TAMANHO];
    int blocoId;
    int qtd_bloco;
    int tam_bloco;

};

void ordena(FILE *f, buffer *buff, int qtd_bloco){
    
}


Endereco* leia(FILE *f, buffer *buff, long indice){
    int indiceBloco;
    long indiceNoBloco;
    
    //Lido com o caso do bloco 0 (se não a segunda linha do else me retornaria sempre 4096 para casos do bloco 0 (indiceBloco nunca seria 0))
    if(indice < TAMANHO){
        indiceBloco = 0;
        indiceNoBloco = indice;
    }
    else{
        indiceBloco = indice / TAMANHO;
        indiceNoBloco = indice - (TAMANHO * indiceBloco);
    }
    

    //Se os ids batem, o bloco na memoria é o bloco que queremos acessar
    if(indiceBloco == buff->blocoId){
        return &buff->endArr[indiceNoBloco];
    }

    //Se não for o bloco que queremos acessar, verificamos se o bloco esta a frente do bloco na memoria ou atrás dele
    //Não precisava desse else if e o else em baixo, mas quis deixar a estrutura igual a o escreva() que é um void aonde nao posso usar o return pra quebrar a execução da função
      
    
    else if (indiceBloco > buff->blocoId){
        //Como meu bloco na memoria esta antes do bloco que eu quero, eu só preciso andar com o carro a diferença de blocos
        int diff = indiceBloco - buff->blocoId;
        //Posso ao invez de dar varios pulos de tamanho fixo, dar um pulo só do offset abaixo
        long offset = diff * buff->tam_bloco;

        fseek(f, offset ,SEEK_CUR);
        fread(buff->endArr, sizeof(Endereco), TAMANHO, f);

        buff->blocoId = indiceBloco;
        
        return &buff->endArr[indiceNoBloco];
    }

    else{
        //Meu bloco esta atrás do atual na memória, então faço um rewind e leio um offset de tamanho do bloco * o indice que eu quero
        long offset = indiceBloco * buff->tam_bloco;
        
        rewind(f);
        fseek(f, offset, SEEK_CUR);
        fread(buff->endArr, sizeof(Endereco), TAMANHO, f);
        
        buff->blocoId = indiceBloco;
        
        return &buff->endArr[indiceNoBloco];
    }
}


void escreva(FILE *f, buffer *buff, long indice, Endereco *e){
    int indiceBloco;
    long indiceNoBloco;

    if(indice < TAMANHO){
        indiceBloco = 0;
        indiceNoBloco = indice;
    }
    else{
        indiceBloco = indice/TAMANHO;
        indiceNoBloco = indice - (TAMANHO * indiceBloco);
    }

    if(indiceBloco == buff->blocoId){
        buff->endArr[indiceNoBloco] = *e;
        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
    }

    else if(indiceBloco > buff->blocoId){
        int diff = indiceBloco - buff->blocoId;
        long offset = diff * buff->tam_bloco;

        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = *e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
    }

    else{
        long offset = indiceBloco * buff->tam_bloco;

        rewind(f);
        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = *e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);

    }


}


/**
Endereco* leiaRecursivo(FILE *f, buffer *buff, long indice, int qtd_bloco){
    int indiceBloco = indice / TAMANHO;
    long indiceNoBloco = indice / indiceBloco;

    if(indiceBloco == buff->blocoId){
        return &buff->endArr[indiceNoBloco];
    }
    
    else if(indiceBloco > buff->blocoId){
        fseek(f, TAM_BLOCO, SEEK_CUR);

        leiaRecursivo(f, buff, indice, qtd_bloco);
    }
    
    else{
        rewind(f);
        leiaRecursivo(f, buff, indice, qtd_bloco);
    }
}

void escrevaRecursivo(FILE *f, buffer *buff, long indice, int qtd_bloco, Endereco *e){
    
}
**/

int main(int argc, char** argv){
    
    FILE *f;
    buffer b;
    long tamanhoArquivo;
    int qtd_bloco;

    
    //Abrindo o arquivos
    f = fopen("C:\\Users\\Zirondi\\Desktop\\cep.dat", "r");

    //Descobrindo o tamanho do arquivo
    fseek(f,0,SEEK_END);
    tamanhoArquivo = ftell(f);
    rewind(f);

    //Definindo o tamanho do bloco e a quantidade de blocos (baseada no tamanho do arquivo)
    b.blocoId = 0;
    b.tam_bloco = sizeof(Endereco) * TAMANHO;
    b.qtd_bloco = tamanhoArquivo / b.tam_bloco;

    fread(b.endArr, sizeof(Endereco), TAMANHO, f);
    //printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",b.endArr[0].logradouro,b.endArr[0].bairro,b.endArr[0].cidade,b.endArr[0].uf,b.endArr[0].sigla,b.endArr[0].cep);
    
    Endereco *e;
    e = leia(f, &b, 4096);

    //std::cout << sizeof(b.endArr[0].cep) << std::endl;
    printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e->logradouro,e->bairro,e->cidade,e->uf,e->sigla,e->cep);

    e = leia(f, &b, 699307);   
    printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e->logradouro,e->bairro,e->cidade,e->uf,e->sigla,e->cep);






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