#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH "/home/tcc/dataset"
#define TAM_NAMEF 256

int remove_directory(const char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}

void listDir(char* path){
    char folder[TAM_NAMEF];
	DIR* dir;
	struct dirent *ent;
    int filesRemoved = 0;

	if((dir=opendir(PATH)) != NULL){
        while (( ent = readdir(dir)) != NULL){
      	    if(ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0  && strcmp(ent->d_name, "..") != 0){
                listDir(ent->d_name);
                strcpy(folder, PATH);
                strcat(folder, "/");
                strcat(folder, ent->d_name);
                filesRemoved = remove_directory(folder);
            }
        }
        printf("Quantidade de Arquivos Apagados: %d\n", filesRemoved);
        closedir(dir);
    }
}

void main(){
  listDir(PATH);
}
