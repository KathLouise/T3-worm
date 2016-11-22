#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <wchar.h>
#include <locale.h>
#define TRUE 1
#define FALSE 0
#define TAM 256
//leitura de diretorio: http://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    
	char dir[TAM],endeArquivo[TAM] ;
	
	FILE *entrada,*saida;
	DIR* D;
	struct dirent* file;
	int i, encontrado;
	wint_t wc;//character utf8
	wint_t palavra[TAM], line[TAM];
	
	if(argc == 1 || argc>2){
        printf("erro, ./worlist nome_diretorio\n");
		return 1;        
    }
	if(strlen(argv[1]) >255){
		printf("erro, numero de caracteres do diretorio maior que 255\n");
        return 1;
	}


    strcpy(dir,argv[1]);	
	//abre o diretorio
    if (NULL == (D = opendir(dir))) 
    {
        printf("Erro ao abrir diretorio\n");
        return 1;
    }


    //abre todos os arquivos do diretorio
	while (file = readdir(D)) 
    {
        //nao pega os diretorios pais ou filhos
        if (!strcmp (file->d_name, "."))
            continue;
        if (!strcmp (file->d_name, ".."))    
            continue;
        
        

        sprintf(endeArquivo,"%s/%s",dir,file->d_name);
        entrada = fopen(endeArquivo, "r");
        if (entrada == NULL)
        {
            printf("erro ao abrir arquivo %s\n",file->d_name);
            return 1;
        }

        //abre o dicionario
		saida = fopen("dicionario.txt","a+");
		if (saida == NULL)
        {
            printf("erro ao abrir dicionario.txt\n");
            fclose(entrada);
			return 1;
        }		
		
		
		//encontra cada palavra no arquivo
		do{
            //zera palavra
            for (i=0;i<sizeof(palavra)/sizeof(wint_t);i++)
                palavra[i]='\0';

            i=0;
            //separa em palavras 
            while ( (wc = fgetwc(entrada)) != WEOF  && ( (wc >= 'A' && wc <= 'Z') || (wc >= 'a' && wc <= 'z') || (wc >= 192) || (wc=='-') ) )  
            {
                palavra[i]=wc;
                
                //remove os '--'
                if (i>0 && palavra[i-1] == '-' && palavra[i] == '-'){
                    i=0;
                    break;
                }
                
                i++;
                //printf("%d\n",i);    
                
            }
            
            
            if (i!=0){
                //procura a palavra no dicionario.txt (saida)
		        
		        rewind(saida);//começa a ler no inicio do arquivo
		        
		        encontrado = FALSE;
                while ( fgetws ( line, TAM , saida) != NULL){
                    line[wcslen(line)-1]='\0'; //elimina o '\n' que eh o ultimo elemento da linha 
                    
                    if (wcscmp(palavra,line) == 0 ){
                        encontrado = TRUE;
                        break;
                    }    
                }
        		//se nao achar a palavra escreve em dicionario
                if (encontrado==FALSE)
                    fwprintf (saida, L"%ls\n",palavra);
                    
            }
        
        
        }while(wc != WEOF);
        
        fclose(saida);
        fclose(entrada);

    }
}
