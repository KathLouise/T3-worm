#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bruteforce.h"
#include "exploit.h"
#include "payload.h"
#include "portscanner.h"
#include "propagation_engine.h"
#include "target_selection.h"

void main(int argc, char *argv[]){
    char range_ipSeq[100];
    char range_portSeq[100];

    if(argc < 3){
        printf("Entrada incorreta.\n\n");
        fflush(stdout);
        printf("A entrada deve seguir o seguinte modelo: ./recon <ip ou range de ips> <porta ou range de portas>\n");
        fflush(stdout);
        exit(0);
    }

    //Pega os ranges
    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);

    initPortScanner(range_ipSeq, range_portSeq);

}
