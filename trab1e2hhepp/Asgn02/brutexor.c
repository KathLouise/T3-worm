#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <openssl/md5.h>

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


//compara o vetor m1 lido do terminal com o m2 obtido da funcao md5
int compara(int *m1,unsigned char *m2,int tam){
	int i;
	for (i=0;i<tam;i++)
		if (m1[i]!=m2[i])
			return -1;
	
	return 0;
}

int main(int argc, char* argv[])
{
    int i;
    char c;
    

	int nMax = 8; //numero máximo de posições
	int n;//numero de caracteres ou posições de uma sequencia
	int m; //numero total de elementos, objetos

	n= 3;
	m = 52;
	
	int seq[nMax];
	//char password[nMax+1];
	
	char password[]="FACIL";
	
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
	
	//for(i=0;i<m;i++)
	//	printf("%c",objetos[i]);
	
	
	//recebe como entrada de stdin como primeira linha o hash usado como teste
	// na segunda linha recebe a cadeia a ser decifrada com um xor
	
	//md5 https://linux.die.net/man/3/md5_init    
    unsigned char md[MD5_DIGEST_LENGTH];    
	unsigned int mdtest[MD5_DIGEST_LENGTH];
	
	
   //lê o hash usado como teste
	for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
        scanf("%2x",&mdtest[i]);
    }

    //lê a cadeia a ser decifrada com o XOR
    unsigned int cadeia[100];
    
    scanf("%2x",&cadeia[0]);
    i=1;
    while(scanf(" %2x",&cadeia[i++])!=EOF);
    
    int tamCadeia=i-1;
    
    	
	//para enumerar todas as sequencias usa-se o algoritmo explicado em:
	//http://www.ime.usp.br/~mms/mac1222s2013/19%20-%20algoritmos%20de%20enumeracao.pdf
	//primeiro cria uma sequencia númerica com n posicoes e m possíveis digitos, de 0 até m-1
	//encontra-se a próxima sequencia somando 1, repete até encontrar todas sequencias
	//ex: n=3 e m=2: começa de 000, e soma 1: 001, 010, até o último 111
	
	//antes de usar o md5 converte-se a sequencia numerica na sequencia de objetos ou letras
	
	for (n=1;n<=nMax;n++){
	
		//cria a primeira sequencia
		for (i=0; i<n; i++) 
    		seq[i] = 0;
    
    	do {      
        	//converte a sequencia numerica na sequencia de letras
        	for (i=0; i<n;i++)
        		password[i]=objetos[seq[i]];
        	password[i]='\0';
        	
            printf("%s\n",password);

            //cria o hash do password
            MD5((unsigned char *)password, strlen(password), md);

            if ( compara(mdtest,md,MD5_DIGEST_LENGTH) ==0){
                printf("a senha é: %s\n",password);    
                
                printf("a cadeia decifrada é: ");
                for (i=0; i<tamCadeia; i++)//faz o xor da senha com a cadeia
            	    printf("%c",password[i%strlen(password)]^cadeia[i]);
            	printf("\n");
                return 0;
            }
    	//gera a próxima sequencia
    	} while (proxima(seq, n, m));
	
	
	}    

}
