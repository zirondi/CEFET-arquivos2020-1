#include <stdio.h>
#include <iostream>
#include <string.h>

#define TAMANHO 50000 //Quantos endereções vão estar no buffer



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
    long qtd_linhas;

};

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
        std::cout << "escrevi no bloco" << std::endl;
    }

    else if(indiceBloco > buff->blocoId){
        int diff = indiceBloco - buff->blocoId;
        long offset = diff * buff->tam_bloco;

        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = *e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
        std::cout << "escrevi no bloco da frente" << std::endl;
    }

    else{
        long offset = indiceBloco * buff->tam_bloco;

        rewind(f);
        fseek(f, offset, SEEK_CUR);

        buff->endArr[indiceNoBloco] = *e;

        fwrite(buff->endArr, sizeof(Endereco), TAMANHO, f);
        std::cout << "escrevi voltando mas escrevi" << std::endl;

    }


}

void swap(FILE *f, buffer *buff, long indice1, long indice2){
    Endereco temp1, temp2;

    temp1 = *(leia(f, buff, indice1));
    temp2 = *(leia(f, buff, indice2));

    escreva(f, buff, indice1, &temp2);
    escreva(f, buff, indice2, &temp1);

    std::cout << "troquei" << std::endl;


}

void heapify(FILE *f, buffer *buff, long indice){
    long raiz = indice;
    long folhaEsquerda = 2 * raiz + 1;
    long folhaDireita = 2 * raiz + 2;
    std::cout << "defini a arvore" << std::endl;

    //char raiz_cep[8], folhaEsquerda_cep[8], folhaDireita_cep[8];
    Endereco end_raiz, end_folhaEsquerda, end_folhaDireita;
    
    
    end_raiz = *(leia(f, buff, raiz));    
    end_folhaEsquerda = *(leia(f, buff, folhaEsquerda));
    end_folhaDireita = *(leia(f, buff, folhaDireita));


    
    



    if(raiz > folhaEsquerda && strncmp(end_folhaEsquerda.cep, end_raiz.cep,8) > 1){
        raiz = folhaEsquerda;
    }

    if(indice > folhaDireita && strncmp(end_folhaDireita.cep, end_raiz.cep,8)>1){
        raiz = folhaDireita;
    }

    if(raiz != indice){
        swap(f, buff, indice, raiz);
        std::cout << "raiz era maior que indice!" << std::endl;
        heapify(f, buff, raiz);
    }


}

void heapsort(FILE *f, buffer *buff){
    std::cout << "entrei no heap" << std::endl;
    for (long i = buff->qtd_linhas / 2 - 1; i >=0; i--){
        std::cout << "entrei no loop indice" << i << std::endl;
        heapify(f, buff, i);
    }

    std::cout << "heap1" << std::endl;

    for (long i = buff->qtd_linhas - 1; i>=0; i--){
        swap(f, buff, 0, i);

        heapify(f, buff, 0);
    }

    std::cout << "heap2" << std::endl;
    
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
    b.qtd_linhas = tamanhoArquivo / sizeof(Endereco);


    fread(b.endArr, sizeof(Endereco), TAMANHO, f);

    heapsort(f, &b);
    //std::cout << strncmp("23027015", "2000000", 8);






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