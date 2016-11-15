#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define TAM_KEY 62

void iterate(char *str, const char *base[], int idx, int len, FILE *output) {
    int i;
    
    if (idx < (len - 1)) {
        for (i = 0; i < TAM_KEY; ++i) {
            str[idx] = *base[i];
            iterate(str, base, idx + 1, len, output);
        }
    }else{
        for (i = 0; i < TAM_KEY; ++i) {
            str[idx] = *base[i];
            fprintf(output, "%s\n", str);
        }
    }

}

void keyGenerator(unsigned int len){
    const char *letters[TAM_KEY] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","X","W","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","x","w","y","z", "0","1","2","3","4","5","6","7","8","9"};
    char concat[8];
    FILE *output;

    output = fopen("chaves.txt","w+");
    if(output == NULL){
        printf("Erro, Nao foi possivel criar o arquivo de saida\n");
        exit(0);
    }

    memset(concat, 0, 8);

    iterate(concat, letters, 0, len, output);
    fclose(output);
}

int bruteforce(unsigned int lenKey){
    
    keyGenerator(lenKey);
    return 1;
}
