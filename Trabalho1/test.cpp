#include<iostream>

#define TAMANHO 4096 //Quantos endereções vão estar no buffer
#define QTD_BLOCO 171


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

using namespace std;

int main(){
    long a = 699306;
    long b = 171;

    long c = a/b;

    cout << c << endl;
}