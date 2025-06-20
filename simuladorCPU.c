#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char memoria[154];

// registradores
unsigned int mbr;
unsigned short int mar, pc, imm, reg[4];
unsigned char ir;
unsigned char ro0, ro1;
unsigned char e, l, g;

// função que converte uma string de mnemônico para seu opcode
int pegarOpcode(char* mnemonico) 
{
    if (strcmp(mnemonico, "hlt") == 0) 
    {
        return 0;
    }

    if (strcmp(mnemonico, "nop") == 0) 
    {
        return 1;
    }

    if (strcmp(mnemonico, "ldr") == 0) 
    {
        return 2;
    }

    if (strcmp(mnemonico, "str") == 0) 
    {
        return 3;
    }
        
    if (strcmp(mnemonico, "add") == 0) 
    {
        return 4;
    }

    if (strcmp(mnemonico, "sub") == 0) 
    {
        return 5;
    }

    if (strcmp(mnemonico, "mul") == 0) 
    {
        return 6;
    }

    if (strcmp(mnemonico, "div") == 0) 
    {
        return 7;
    }

    if (strcmp(mnemonico, "cmp") == 0) 
    {
        return 8;
    }

    if (strcmp(mnemonico, "movr") == 0) 
    {
        return 9;
    }

    if (strcmp(mnemonico, "and") == 0) 
    {
        return 10;
    }

    if (strcmp(mnemonico, "or") == 0) 
    {
        return 11;
    }

    if (strcmp(mnemonico, "xor") == 0)
    {
        return 12;
    }

    if (strcmp(mnemonico, "not") == 0)
    {
        return 13;
    }

    if (strcmp(mnemonico, "je") == 0)
    {
        return 14;
    }

    if (strcmp(mnemonico, "jne") == 0) 
    {
        return 15;
    }

    if (strcmp(mnemonico, "jl") == 0)
    {
        return 16;
    }

    if (strcmp(mnemonico, "jle") == 0)
    {
        return 17;
    }

    if (strcmp(mnemonico, "jg") == 0)
    {
        return 18;
    }

    if (strcmp(mnemonico, "jge") == 0) 
    { 
        return 19;
    }

    if (strcmp(mnemonico, "jmp") == 0) 
    {
        return 20;
    }

    if (strcmp(mnemonico, "ld") == 0) 
    {
        return 21;
    }

    if (strcmp(mnemonico, "st") == 0)
    {
        return 22;
    }

    if (strcmp(mnemonico, "movi") == 0)
    {
        return 23;
    }

    if (strcmp(mnemonico, "addi") == 0) 
    {
        return 24;
    }

    if (strcmp(mnemonico, "subi") == 0) 
    {
        return 25;
    }

    if (strcmp(mnemonico, "muli") == 0) 
    {
        return 26;
    }

    if (strcmp(mnemonico, "divi") == 0)
    {
        return 27;
    } 

    if (strcmp(mnemonico, "lsh") == 0) 
    {
        return 28;
    }

    if (strcmp(mnemonico, "rsh") == 0) 
    {
        return 29;
    }

    return -1; // opcode não encontrado
}

// função que converte uma string de registrador para seu numero
int numeroRegistrador(char* str_reg) {
    if (str_reg == NULL) {
        return -1;
    }
    return atoi(str_reg + 1); // pula o 'r' e converte o resto para inteiro
}

void carregarArquivo (const char* nomeArquivo) // endereço;tipo;conteúdo 
{
    FILE* arquivo;
    char linha[100];
    char* pedaco;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) // verifica se o arquivo existe
    {
        printf("nao foi possivel abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) // leitura por linha
    {
        // pega o endereço em hexa
        pedaco = strtok(linha, ";");
        if (pedaco == NULL) continue;

        long int endereco = strtol(pedaco, NULL, 16);

        // pega o tipo - i para instrução, d para dado
        pedaco = strtok(NULL, ";");
        if (pedaco == NULL) continue;

        char tipo = pedaco[0];

        // pega o conteúdo (a instrução ou o dado)
        pedaco = strtok(NULL, ";\n");
        if (pedaco == NULL) continue;

        char* conteudo = pedaco;

        if (tipo == 'i')  // tipo instrução
        {
            char* mnemonico = strtok(conteudo, " ,"); // pega o mnemonico

            if (mnemonico == NULL) continue;

            int opcode = pegarOpcode(mnemonico);

            // inicia com -1 para sinalizar que o operando não foi encontrado
            int numReg1 = -1, numReg2 = -1;  // variáveis temp que ajudam a função a fazer a tradução
            long int enderecoOUimm = -1;

            // pega o resto dos operandos
            char* operando1 = strtok(NULL, " ,");

            if (operando1) 
            {
                numReg1 = numeroRegistrador(operando1);

                char* operando2 = strtok(NULL, " ,");

                if (operando2) 
                {
                    if (operando2[0] == 'r') 
                    {
                        numReg2 = numeroRegistrador(operando2);
                    } 
                    else 
                    {
                        enderecoOUimm = strtol(operando2, NULL, 16);
                    }
                }
            }
            
            // monta a palavra de instrução baseada no opcode
            if (opcode >= 0 && opcode <= 1) // 1 byte
            { 
                memoria[endereco] = (opcode << 3);
            } 

            else if ((opcode >= 2 && opcode <= 12) || opcode == 13) // 2 bytes
            { 
                memoria[endereco] = (opcode << 3) | (numReg1 << 1); 
                memoria[endereco+1] = (numReg2 != -1) ? (numReg2 << 6) : 0;
            }

            else // 3 bytes
            { 
                 memoria[endereco] = (opcode << 3) | (numReg1 << 1);
                 memoria[endereco+1] = (enderecoOUimm >> 8) & 0xFF; // byte alto
                 memoria[endereco+2] = enderecoOUimm & 0xFF;      // byte baixo
            }
        }

        else if (tipo == 'd') // tipo dado
        { 
            unsigned short int dado = strtol(conteudo, NULL, 16);

            memoria[endereco] = (dado >> 8) & 0xFF; // byte de cima
            memoria[endereco+1] = dado & 0xFF;      // byte de baixo
        }
    }

    fclose(arquivo);
    printf("arquivo '%s' carregado na memoria\n", nomeArquivo);
}


void busca() 
{
    
    mar = pc; // endereço da instrução do pc -> mar
    mbr = memoria[mar]; // busca o primeiro byte da memoria e armazena no mbr
    pc++; // pc aponta para o prox byte
 
    ir = mbr >> 3; // extrai os 5 bits do opcode, depois desloca 3 bits a direita


    // instrução de 1 byte
    if (ir == 0 || ir == 1 || ir == 13)
    {
        // instrução já esta completa no mbr e o pc ja foi incrementado
    }

    // instrução de 2 bytes
    else if (ir >= 2 && ir <= 12) 
    {

        mar = pc; // aponta para o segundo byte
        mbr = (mbr << 8) | memoria[mar]; 
        pc++;
    }
    
    // instrução de 3 bytes
    else 
    {
        // busca e guarda o segundo byte
        mar = pc;
        mbr = (mbr << 8) | memoria[mar];
        pc++;

        // busca e guarda o terceiro byte
        mar = pc;
        mbr = (mbr << 8) | memoria[mar];
        pc++;
    }
}

void decodifica()
{

    // instruções de 1 byte
    if (ir == 0 || ir == 1) 
    {

    }

    // instruções de 2 bytes
    else if (ir >= 2 && ir <= 13)
    {
        // decodifica reg0 para todas
        ro0 = (mbr & 0x0600) >> 9;

        // decodifica reg1 so se não for a instrução not
        if (ir != 13) 
        {
            ro1 = (mbr & 0x0180) >> 7;
        }
    }

    // instruções de 3 bytes
    else
    {
        // endereço
        if (ir >= 14 && ir <= 20)
        {
            mar = mbr & 0xFFFF;
        }

        // reg0 e endereço
        else if (ir == 21 || ir == 22)
        {
            ro0 = (mbr & 0x060000) >> 17;
            mar = mbr & 0xFFFF;
        }

        // reg0 e imediato
        else if (ir >= 23 && ir <= 29)
        {
            ro0 = (mbr & 0x060000) >> 17;
            imm = mbr & 0xFFFF;
        }
    }
}

void executa() {
    
    if (ir == 0) // hlt
    { 
        printf("fim da execucao\n");
    }

    else if (ir == 1) // nop
    { 
        
    }

    else if (ir == 2) // ldr rx, ry
    { 
        mar = reg[ro1];

        mbr = memoria[mar];
        mar++;
        mbr = (mbr << 8) | memoria[mar];

        reg[ro0] = mbr;
    }

    else if (ir == 3) // str rx, ry
    { 
        mar = reg[ro1];

        memoria[mar] = (reg[ro0] >> 8) & 0xFF;
        mar++;
        memoria[mar] = reg[ro0] & 0xFF;
    }

    else if (ir == 4) // add rx, ry
    { 
        reg[ro0] = reg[ro0] + reg[ro1];
    }

    else if (ir == 5) // sub rx, ry
    { 
        reg[ro0] = reg[ro0] - reg[ro1];
    }

    else if (ir == 6) // mul rx, ry
    { 
        reg[ro0] = reg[ro0] * reg[ro1];
    }

    else if (ir == 7) // div rx, ry
    {
        if (reg[ro1] != 0) 
        { 
            reg[ro0] = reg[ro0] / reg[ro1]; 
        }
    }

    else if (ir == 8) // cmp rx, ry
    { 
        e = (reg[ro0] == reg[ro1]);
        l = (reg[ro0] < reg[ro1]);
        g = (reg[ro0] > reg[ro1]);
    }

    else if (ir == 9) // movr rx, ry
    { 
        reg[ro0] = reg[ro1];
    }

    else if (ir == 10) // and rx, ry 
    { 
        reg[ro0] = reg[ro0] & reg[ro1];
    }

    else if (ir == 11) // or rx, ry
    { 
        reg[ro0] = reg[ro0] | reg[ro1];
    }

    else if (ir == 12) // xor rx, ry 
    { 
        reg[ro0] = reg[ro0] ^ reg[ro1];
    }

    else if (ir == 13) // not rx 
    { 
        reg[ro0] = ~reg[ro0];
    }

    else if (ir == 14) // je z
    { 
        if (e == 1) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 15) // jne z
    { 
        if (e == 0) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 16) // jl z 
    { 
        if (l == 1) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 17) // jle z 
    { 
        if (l == 1 || e == 1) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 18) // jg z 
    { 
        if (g == 1) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 19) // jge z 
    { 
        if (g == 1 || e == 1) 
        { 
            pc = mar; 
        }
    }

    else if (ir == 20) // jmp z 
    { 
        pc = mar;
    }

    else if (ir == 21) // ld rx, z
    { 
        mbr = memoria[mar];

        mar++;
        mbr = (mbr << 8) | memoria[mar];

        reg[ro0] = mbr;
    }

    else if (ir == 22) // st rx, z 
    { 
        memoria[mar] = (reg[ro0] >> 8) & 0xFF;
        mar++;
        memoria[mar] = reg[ro0] & 0xFF;
    }

    else if (ir == 23) // movi rx, imm
    { 
        reg[ro0] = imm;
    }

    else if (ir == 24) // addi rx, imm
    { 
        reg[ro0] = reg[ro0] + imm;
    }

    else if (ir == 25) // subi rx, imm
    { 
        reg[ro0] = reg[ro0] - imm;
    }

    else if (ir == 26) // muli rx, imm
    { 
        reg[ro0] = reg[ro0] * imm;
    }

    else if (ir == 27) // divi rx, imm 
    { 
        if (imm != 0) 
        { 
            reg[ro0] = reg[ro0] / imm; 
        }
    }

    else if (ir == 28) // lsh rx, imm
    { 
        reg[ro0] = reg[ro0] << imm;
    }

    else if (ir == 29) // rsh rx, imm
    { 
        reg[ro0] = reg[ro0] >> imm;
    }

    else 
    {
        printf("opcode %d nao reconhecido\n", ir);
    }
}

int main(){ 
    
    // função para ler um arq .txt 

    // while . .
    busca();
    decodifica();
    executa();

    
} 