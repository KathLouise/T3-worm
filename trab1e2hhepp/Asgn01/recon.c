#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/socket.h> 
#include<unistd.h>
#include<arpa/inet.h> //inet_addr
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>

int main(int argc, char* argv[])	
{
    int i,j,k,tam;
    char IP[32],port[32],temp[10];
    char endIP[32];
    int ipIn,ipFin, portIn, portFin;
    
    char buffer[100];
    
    
    const time_t timer = time(NULL);

  
    strftime (buffer, sizeof(buffer), "Varredura iniciada em %a %d %b %Y %T %Z\n", localtime (&timer));
    printf ("%s",buffer);
    
    
     //Sex 19 Ago 2016 13:21:33 BRT
    
    //=============leitura dos IPs e portas=====================================
    
    if(argc == 1 || argc>3){
        printf("./recon endereçoIP (ou faixa separada por -) e, opcionalmente, uma porta (ou intervalo separado por -)\n");
        printf("Ex: ./recon 192.168.1.1-10 1-1023\n");
        exit;
    }
    //identifica os IPs
    
    printf("IP: %s\n",argv[1]);
    
    tam = strlen(argv[1]);
    for(i=0;i<tam;i++){
        if(argv[1][i]=='-')
            break;
        IP[i]=argv[1][i];
    }
    IP[i]='\0';
    
    //Se foi dado um intervalo de ip
    if(i<tam){
        k=0;
        for (j=i+1;j<tam;j++){
            temp[k++] = argv[1][j];
        }
        temp[k]='\0';
        ipFin = atoi(temp);
    }else ipFin = 0;

    //recorta a parte final do IP e cola em ipIn
    
    int npontos;
    npontos=0;
    tam = strlen(IP);
    k=0;
    for(i=0;i<tam;i++){
        if (npontos == 3){
            temp[k++]=IP[i];
            IP[i]='\0';
        }    
        if(IP[i]=='.')
            npontos++;
    }
    temp[k]='\0';
    
    //ATENCAO: SÓ FUNCIONA COM IPv4
    ipIn =atoi(temp);
    if (ipFin==0)
        ipFin=ipIn;
    
    //printf("%s %d %d\n",IP,ipIn,ipFin);
    
    //se não for dada uma porta o programa escaneia todas as portas possiveis
    if (argc == 2){
        portIn = 1;
        portFin = 65535;
        printf("Portas: %d-%d\n",portIn,portFin);
    }else //identifica o intervalo das portas
    {
        printf("Portas: %s\n",argv[2]);
        tam = strlen(argv[2]);
        for(i=0;i<tam;i++){
            if(argv[2][i]=='-')
                break;
            port[i]=argv[2][i];
        }
        port[i]='\0';
        portIn = atoi(port);
        if(i<tam){
            k=0;
            for (j=i+1;j<tam;j++){
                temp[k++] = argv[2][j];
            }
            temp[k]='\0';
            
            portFin = atoi(temp);
        }
        else portFin = portIn;
    }

    

    //=====término leitura===========
    
    //declara o socket
    int sock;
    //cria a struct que recebe o endereço IP e a Porta
    struct sockaddr_in servidor;
    
    /* descrição da struct
     http://www.binarytides.com/socket-programming-c-linux-tutorial/
    // IPv4 AF_INET sockets:
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };
    struct in_addr {
        unsigned long s_addr;          // load with inet_pton() 
    };
    struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };
    */
    int c;

    struct timeval tv2;
    tv2.tv_sec = 60;  
    tv2.tv_usec = 0;  

    
    for (i=ipIn;i<=ipFin;i++){
        
        
        sprintf(endIP,"%s%d",IP,i);
        //printf("%s\n",endIP);
        
        //coloca o endereço do IP na struct
        servidor.sin_addr.s_addr = inet_addr(endIP);
        //endereço IPv4
        servidor.sin_family = AF_INET;
        
        for (j=portIn;j<=portFin;j++){
            
           //coloca a porta na struct
            servidor.sin_port = htons(j);
            
            //cria o socket
            sock = socket(AF_INET , SOCK_STREAM , 0);
            //AF_INET == endereço IPv4
            //SOCK_STREAM == conexão tipo TCP
            // 0 == protocolo padrão do socket, nesse caso, o IP
            if (sock < 0)
            {
                printf("Erro ao criar socket");
                exit;
            }


            
            //conecta com o servidor
            c = connect(sock,(struct sockaddr *)&servidor,sizeof(servidor));
            
            //printf("%d",c);
            
            if(c == 0){
				setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv2,sizeof(struct timeval));
                //zera buffer
                for (k=0;k<sizeof(buffer);k++)
                    buffer[k]='\0';
                
                read(sock,buffer,sizeof(buffer));
                
                printf("%s\t%d\t%s\n",endIP,j,buffer);
            
            }
            //else
            //    printf ("Error %s\n",strerror(errno));
            close(sock);
        }
    }

}
