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
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include "portscanner.h"

//-------------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------------//

#define MAX_IP 16
#define MAX_PORT 65536

//-------------------------------------------------------------------//
// FUNÇÕES
//-------------------------------------------------------------------//
// Função que valida o endereço de ip

int validateIPAddr(char *ip){
    struct sockaddr_in socket;
    
    int result = inet_pton(AF_INET, ip, &(socket.sin_addr));
    
    return result;
}
//-------------------------------------------------------------------//
// Função que transforma um inteiro em char
// É dependente da base do inteiro, para efetuar a conversão correta

char* itoa(int val, int base){
	static char buf[32] = {0};
	int i = 30;
	
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}
//-------------------------------------------------------------------//
// Função que particiona a string dependendo do delimitador passado
// Divide a string em vários tokens se o delimitador passado estiver 
// presente na string passada como parametro

void createToken(char *range[],  char string[], const char delimiter[], int *cont){
    int i = 0;
    char *token;
    
    token = strtok(string, delimiter);
    
    while(token != NULL){
        range[i] = token;
        token = strtok(NULL, delimiter);
        i++;
    }
    range[i] = token;
    *cont = i;
}
//-------------------------------------------------------------------//
// Função que constroi o range de IP
// Primeiro: particiona-se a string do IP de entrada em 2 (IP de inicio 
// e os tres ultimos digitos do IP final).
// Segundo: particiona o IP de inicio em vários tokens, e monta-se os 
// 3 primeiros campos do endereço.
// Terceiro: a partir do final do ip de inicio, gera-se todos os ultimo 
// tres digitos daquele intervalo e guarda em um vetor.

void constructIPRange(char range_ipSeq[], char *rangeTotal[], int *numIP){
    const char delimiter[2] = "-";
    const char delimiter2[2] = ".";
    char *token, *token2, *addr;
    char *range_ip[MAX_IP] = {0};
    char *range[MAX_IP] = {0};
    char *range2[MAX_IP] = {0};
    char first_ip[MAX_IP] = {0};
    char *finalIPBand;
    char *ponto = ".";
    char ip_aux[MAX_IP] = {0};
    char ip[MAX_IP] = {0};
    char range_ips[255] = {0};
    int i = 0, j, tamInit, tamFinal, initIP, finalIP;
    
    createToken(range_ip, range_ipSeq, delimiter, &i);
    
    finalIPBand = range_ip[1];
    i = 0;
    createToken(range, range_ip[0], delimiter2, &i);
    tamInit = i-1;
    
    i = 0;
    createToken(range2, finalIPBand, delimiter2, &i);
    tamFinal = i-1;

    for(i = 0; i < MAX_IP; i++){
        if(i < tamInit){
            strcpy(ip_aux, range[i]);
            strcat(ip_aux, ponto);
            strcat(ip, ip_aux);
        }
    }
    
    if((!isdigit(*range[tamInit])) || (!isdigit(*range2[0]))){
        printf("Digito inicial ou final do range é desconhecido. \n");
        fflush(stdout);
        printf("Tente novamente, fornecendo somente numeros: XXX.XXX.XXX.XXX .\n");
        fflush(stdout);
        exit(1);
    }
    
    initIP = atoi(range[tamInit]);
    if(range2[0] == NULL){
        finalIP = initIP;
    }else{
        finalIP = atoi(range2[tamFinal]);
    }
    
    if(finalIP < initIP){
        printf("IP final é inferior ao IP de inicio. \n");
        fflush(stdout);
        printf("Por favor entre com um intervalo valido, por exemplo: XXX.XXX.XXX.1-10\n");
        fflush(stdout);
        exit(1);
    }
    
    j = 0;
    if(range2[0] == NULL){
        strcat(ip, range[tamInit]);
        rangeTotal[0] = strdup(ip);
    }else{
        for(i = initIP; i <= finalIP; i++){
            if(i == 0){
                ip_aux[0] = '\0';
                strcpy(ip_aux, ip);
                strcat(ip_aux, range[tamInit]);
                rangeTotal[0] = strdup(ip_aux);
                j++;
            }else{
                ip_aux[0] = '\0';
                addr = itoa(i,10);
                strcpy(ip_aux, ip);
                strcat(ip_aux, addr);
                rangeTotal[j] = strdup(ip_aux);
                j += 1;
            }
        }
    }
    
    *numIP = j;
}
//-------------------------------------------------------------------//
// Função que constroi o range de portas
// Primeiro: quebra a string inicial em tokens
// Segundo: transforma o token de inicio e o token de fim em inteiros
// Terceiro: gera uma lista de inteiros, para guardar todas as 
// portas naquele intervalo

void constructPortRange(char range_portSeq[], unsigned int range[], int *numPort){
    int i = 0, j = 0;
    char *token;
    const char delimiter[2] = "-";
    unsigned int port[2];
    unsigned int init, final;

    token = strtok(range_portSeq, delimiter);

    while(token != NULL){
        if(!isdigit(*token)){
            printf("Range de porta invalida.\n");
            fflush(stdout);
            exit(1);
        }
        port[i] = (unsigned int) atoi(token);
        token = strtok(NULL, delimiter);
        i++;
    }
    
    init = port[0];
    final = port[1];
    
    if(init > MAX_PORT || final > MAX_PORT){
        printf("Range de porta extrapolou o valor permitido.\n");
        fflush(stdout);
        exit(1);
    }
    if(init > final){
        printf("Valor da Porta Inicial é maior que o valor da Porta Final.\n");
        fflush(stdout);
        exit(1);
    }

    for(i = init; i <= final; i++){
        range[j] = i;
        j += 1;
    }
    *numPort = j;
    
}
//-------------------------------------------------------------------//
// Função que faz o scan da Porta
// Primeiro: Abre o socket em protocolo TCP
// Segundo: Seta os paramentos e faz o 3-way-handshake para 
// pedido de conexão
// Terceiro: Se conectar, ai pede o banner.

int connectIP(unsigned int port_number, char *ip, FILE *output){
    struct sockaddr_in server_addr;
    int sock, try_connect, recv, send;
    char buffer[1024] = {0};

    //cria o socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock < 0){
        printf("Erro ao criar o socket.\n");
        fflush(stdout);
        exit(1);    
    }
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);

    //Conectando ao server
    try_connect = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));


    if(try_connect >= 0){
        sleep(2);

        send = write(sock, "ack", strlen("ack"));
        if(send < 0){
            printf("%s\t %d\n", ip, port_number);
            fflush(stdout);
            printf("Erro ao escrever no socket.\n\n");
        }
        
        bzero(buffer, 255);

        recv = read(sock, buffer, 1023);
        
        if(recv >= 0){
            fprintf(output, "%s %d %s", ip, port_number, buffer);
            printf("%s %d %s\n", ip, port_number, buffer);
        }
    }
    
    if(sock > 1){
        close(sock);
    }
}
//-------------------------------------------------------------------//
// Função main
// Faz a construção ou atribuição dos IPs e das Portas
// Chama a função de validação de endereço ip e a de conexão

void initPortScanner(char range_ipSeq[], char range_portSeq[]){
    char *range_ip[256] = {0};
    char *isIPRange, *isPortRange;
    char first_port[MAX_PORT] = {0};
    unsigned int range_port[MAX_PORT];
    int numPort = 0, numIP = 0, i, j, isValidIP, hasIPRange = 1, hasPortRange = 1, sock;
    time_t r_time;
    struct tm *info;
    FILE *output;

    output = fopen("varredura.txt", "w+");
    if(output == NULL){ 
        printf("Erro: Nao foi possivel criar o arquivo de saida\n");
        exit(0);
    }

    //Cuidando do range de IP/IP
    
    isIPRange = strchr (range_ipSeq, '-');
    if(isIPRange != NULL){
        constructIPRange(range_ipSeq, range_ip, &numIP);
    }else{
        if(isdigit(range_ipSeq[1])){
            if(range_ipSeq[1] != '\0'){
                range_ip[0] = range_ipSeq;
                numIP = 1;
            }
        }else{
            printf("Caracter de separação invalido, ou range desconhecido. \n");
            fflush(stdout);
            printf("Tente novamente, separando seu range por - .\n");
            fflush(stdout);
            exit(1);
        }
    }
    
    //Cuidando do range de Porta/Porta
    isPortRange = strchr (range_portSeq, '-');
    if(isPortRange != NULL){
        constructPortRange(range_portSeq, range_port, &numPort);
    }else{
        if(isdigit(range_portSeq[0])){
            if(!isdigit(range_portSeq[0])){
                printf("Porta inválida.\n");
                fflush(stdout);
                exit(1);
            }
            if(atoi(range_portSeq) > MAX_PORT){
                printf("Range de porta extrapolou o valor permitido.\n");
                fflush(stdout);
                exit(1);
            }
            range_port[0] = (unsigned int) atoi(range_portSeq);
            numPort = 1;
        }else{
            printf("Caracter de separação invalido, ou range desconhecido. \n");
            fflush(stdout);
            printf("Tente novamente, separando seu range por - .\n");
            fflush(stdout);
            exit(1);
        }
    }

    //Verifica o IP e faz a conexão, se ele for valido
    for(i = 0; i < numIP; i++){
        isValidIP = validateIPAddr(range_ip[i]);
        if(isValidIP){
            for(j = 0; j < numPort; j++){
                sock = connectIP(range_port[j], range_ip[i], output);
            }
        }else{
            printf("IP inválido.\n");
            fflush(stdout);
        }
    }
    
    fclose(output);
}
//-------------------------------------------------------------------//
