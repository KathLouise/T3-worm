//-------------------------------------------------------------------//
// UNIVERSIDADE FEDERAL DO PARANÁ                                    //
// HENRIQUE HEPP						     //
// KATHERYNE LOUISE GRAF     					     //
// CI301 - INTRODUÇÃO À SEGURANÇA COMPUTACIONAL                      //
//-------------------------------------------------------------------//
// BIBLIOTECAS
//-------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//-------------------------------------------------------------------//
// FUNÇÕES
//-------------------------------------------------------------------//
// Esta função escolhe um numero aleatóriamente, entre o número total
// de linhas contidas no arquivo, e salva o ip e a porta

void randomLine(char **lines, char **param, int numberLines){
    int random, i = 0;
    const char delimiter[2] = " ";
    char *token;
    
    srand(time(NULL));
    random = rand() % numberLines;

    token = strtok(lines[random], delimiter);
    while(token != NULL && i < 2){
        param[i] = malloc(strlen(token) + 1);
        strcpy(param[i], token);
        token = strtok(NULL, delimiter);
        i += 1;
    }
    
}
//-------------------------------------------------------------------//
// Esta função é o "main" do target_selection, nela, o arquivo gerado
// pelo portscanner com os IPs, é lido e tem seu numero de linhas 
// contado, para que depois a randomLine possa escolher uma delas
// de forma aleatória

void target_selection(char **param){
    FILE *input;
    char *line = NULL;
    char **lineArray;
    size_t len = 0;
    ssize_t read;
    int numberLines = 0, i = 0, ch = 0;

    input = fopen("varredura.txt", "r+");
    if(input == NULL){
        printf("Erro ao tentar abrir o arquivo. \n");
        exit(0);
    }

    while((read = getline(&line, &len, input)) != -1){
        numberLines += 1;
    }

    if(numberLines == 0){
        printf("Arquivo de varredura esta vazio. \n");
        exit(0);
    }else{
        rewind(input);
        lineArray = malloc(numberLines*sizeof(char *));
        //param = malloc(2*sizeof(char *));

        while((read = getline(&line, &len, input)) != -1){
            lineArray[i] = malloc(strlen(line) + 1);
            strcpy(lineArray[i], line);
            i += 1;
        }
    
        randomLine(lineArray, param, numberLines);

        free(lineArray);
    }

    fclose(input);
    free(line);
}
//-------------------------------------------------------------------//
