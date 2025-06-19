#include <stdio.h>
#include <stdlib.h>

// vetor de palavras de oito bits de 154 posições 
unsigned char memoria[154];

// registradores
unsigned int mbr;
unsigned short int mar, pc, imm;
unsigned char ir;
unsigned char ro0, ro1;
unsigned char e, l, g;

void busca(){
    
}

void executa(unsigned char opcode, unsigned char endereco);
    // vc deve testar qual é a instrução com base no opcode, e chama a função certa
    // org -> switch

int main(){ 
    
    // função para ler um arq .txt 

    // while . .
    busca();
    decodifica();
    executa();

    
} 