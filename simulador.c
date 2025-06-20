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

// converte uma string de mnemônico para seu opcode
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

// converte uma string de registrador para seu numero
int numeroRegistrador(char* str_reg) {
    if (str_reg == NULL) {
        return -1;
    }
    return atoi(str_reg + 1); // pula o 'r' e converte o resto para inteiro
}

void carregarMemoria(const char* nomeArquivo) {
    FILE* arquivo;
    char linha[100];
    char* pedaco;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) 
    {
        printf("erro em abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    // le o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo) != NULL) 
    {

        // pega o endereço em hexa
        pedaco = strtok(linha, ";");
        if (pedaco == NULL) continue;
        long int endereco = strtol(pedaco, NULL, 16);

        // pega o tipo (i para instrução ou d para dado)
        pedaco = strtok(NULL, ";");
        if (pedaco == NULL) continue;
        char tipo = pedaco[0];

        // pega o conteudo 
        pedaco = strtok(NULL, ";\n");
        if (pedaco == NULL) continue;
        char* conteudo = pedaco;

        if (tipo == 'i')  // tipo instrução
        {
            char* mnemonico = strtok(conteudo, " ,");
            if (mnemonico == NULL) continue;

            int opcode = pegarOpcode(mnemonico);
            if (opcode == -1) 
            {
                printf("mnemonico '%s' desconhecido\n", mnemonico);
                continue;
            }

            // var temporárias 
            int numReg1 = -1, numReg2 = -1;
            long int enderecoOUimm = -1;

            // pega o primeiro operando
            char* op1 = strtok(NULL, " ,");
            if (op1) 
            {
                if (op1[0] == 'r') 
                {
                    numReg1 = numeroRegistrador(op1);
                } 
                else 
                {
                    enderecoOUimm = strtol(op1, NULL, 16);
                }
            }

            // pega o segundo operando
            char* op2 = strtok(NULL, " ,");
            if (op2) 
            {
                if (op2[0] == 'r') 
                {
                    numReg2 = numeroRegistrador(op2);
                } 
                else 
                {
                    enderecoOUimm = strtol(op2, NULL, 16);
                }
            }

            // instruções de 1 byte
            if (opcode <= 1) 
            {
                memoria[endereco] = (unsigned char)(opcode << 3);
            }

            // instruções de 2 bytes 
            else if (opcode <= 13) 
            {
                unsigned short int palavra = 0; // palavra de 16 bits

                // monta a palavra de 16 bits completa
                palavra = (opcode << 11) | (numReg1 != -1 ? (numReg1 << 9) : 0) | (numReg2 != -1 ? (numReg2 << 7) : 0);

                // armazena os 2 bytes na memoria
                memoria[endereco] = (palavra >> 8) & 0xFF; // byte de cima
                memoria[endereco + 1] = palavra & 0xFF; // byte de baixo
            }

            // instruções de 3 bytes 
            else 
            {
                unsigned int palavra = 0; // palavra de 24 bits 
                
                if (opcode >= 14 && opcode <= 20) // jumps
                { 
                    palavra = (opcode << 19) | (enderecoOUimm & 0xFFFF);
                } 
                else // ld, st, movi, addi, etc
                { 
                    palavra = (opcode << 19) | (numReg1 != -1 ? (numReg1 << 17) : 0) | (enderecoOUimm & 0xFFFF);
                }

                // armazena os 3 bytes na memória
                memoria[endereco] = (palavra >> 16) & 0xFF; // byte mais alto
                memoria[endereco + 1] = (palavra >> 8) & 0xFF; // byte do meio
                memoria[endereco + 2] = palavra & 0xFF; // byte mais baixo
            }
        }

        else if (tipo == 'd') // tipo dado
        {
            // dados empre ocupam 2 bytes (16 bits)
            unsigned short int dado = strtol(conteudo, NULL, 16);

            memoria[endereco] = (dado >> 8) & 0xFF; // byte de cima
            memoria[endereco + 1] = dado & 0xFF; // byte de baixo
        }
    }

    fclose(arquivo);
    printf("arquivo '%s' carregado na memoria.\n", nomeArquivo);
}

void busca() 
{
    
    mar = pc; // endereço da instrução do pc -> mar
    mbr = memoria[mar]; // busca o primeiro byte da memoria e armazena no mbr
    pc++; // pc aponta para o prox byte
 
    ir = mbr >> 3; // extrai os 5 bits do opcode, depois desloca 3 bits a direita


    // instrução de 1 byte
    if (ir == 0 || ir == 1)
    {
        // nada a fazer
    }

    // instrução de 2 bytes
    else if ((ir >= 2 && ir <= 12) || ir == 13)
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

void estadoCpu() {
    printf("-------------------------------------------------\n");
    printf("CPU:\n");
    printf("RO: %04X  R1: %04X  R2: %04X  R3: %04X\n", reg[0], reg[1], reg[2], reg[3]);
    printf("MBR: %08X                PC: %04X\n", mbr, pc);
    printf("MAR: %04X                 IMM: %04X\n", mar, imm);
    printf("IR: %02X       RO0: %X         RO1: %X\n", ir, ro0, ro1);
    printf("E: %X        L: %X         G: %X\n", e, l, g);
    printf("-------------------------------------------------\n");
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        printf("erro: o programa precisa de um nome de arquivo como argumento\n");
        printf("formato: %s <programa.txt>\n", argv[0]);
        return 1; // Encerra o programa
    }

    carregarMemoria(argv[1]);

    printf("------------------------------------------------------\n");
    printf("                  CPU Simulator \n");
    printf("------------------------------------------------------\n\n");

    int rodando = 1; // flag: 1 = rodando, 0 = parar

    printf("PRESSIONE ENTER PARA INICIAR\n");
    getchar();

    while (rodando) {
        
        estadoCpu();

        printf("\npressione ENTER para iniciar o proximo ciclo ou CTRL+C para finalizar a execucao\n");
        getchar();

        printf("\nINICIANDO O PROXIMO CICLO:\n"); 

        busca();
        decodifica();
        executa();

        // verifica condição de parada
        // se a instrução for hlt, a execução deve parar
        if (ir == 0) {
            rodando = 0;
        }
    }

    printf("\ninstrucao HLT executada: FIM DE EXECUCAO\n");
    estadoCpu();

    return 0; 
}