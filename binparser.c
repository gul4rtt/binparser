#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYTE_SIZE 16

void usage(void){
    puts("uso: ./readpe arquivo.exe");
}

void error(char *msg){
    if(msg != NULL)
        fprintf(stderr, "%s\n", msg);

    exit(1);
}

// imprime o conteúdo do buffer
void printBuffer(unsigned char buffer[], unsigned int setter) {
    if(setter == 1) {
        for(int i = 0; i < BYTE_SIZE; i++) {
            printf("%02x ", buffer[i]);
            if(i == BYTE_SIZE/2-1)
                printf("   ");
        }
    } else {
        for(int i = 0; i < BYTE_SIZE; i++) {
            if(isprint(buffer[i])){
                printf("%c", buffer[i]);
            }else{
                printf(".");
            }
        }
    }
}

// identifica o tipo de arquivo
int binCheck(unsigned char buffer[]){
    if(buffer[0] == 0x4d && buffer[1] == 0x5a)
        return 0;
    else if(buffer[1] == 0x45 && buffer[2] == 0x4c && buffer[3] == 0x46)
        return 1;
    else 
        return 2;
    
}

int main(int argc, char *argv[]){
    FILE *fh;
    unsigned char buffer[BYTE_SIZE];
    if(argc != 2){
        usage();
        error(NULL);
    }

    if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
        usage();
        exit(0);
    }

    fh = fopen(argv[1], "rb");

    if(fh == NULL)
        error("não foi possível abrir o binário");

    if(fread(buffer, BYTE_SIZE, 1, fh) != 1)
        error("não foi possivel a leitura total do arquivo");

    switch(binCheck(buffer)){
        case 0:
            puts("Tipo de arquivo [PE]\n");
            break;
        case 1: 
            puts("Tipo de arquivo [ELF]\n");
            break;
        default:
            puts("Tipo de arquivo [Não Identificado]\n");
            break;
    }

    rewind(fh);

    // realiza a leitura e impressão do arquivo 
    while((fread(buffer, 1, BYTE_SIZE, fh)) > 0){
        printBuffer(buffer, 1);
        printf("| ");
        printBuffer(buffer, 2);
        puts("");
    }

    fclose(fh);
    return 0;
}
