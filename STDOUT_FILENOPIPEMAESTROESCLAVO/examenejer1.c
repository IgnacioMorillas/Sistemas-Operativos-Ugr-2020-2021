//EXAMEN 2012
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){
  if(argc !=3){
    perror("SOLO 2 ARGUMENTOS");
    exit(1);
  }
  int fd;
  DIR *d;
  char ruta[512];
  struct stat st;
  struct dirent *ent;

  if ((d = opendir(argv[1])) == NULL){
        perror("ERROR AL ABRIR EL DIRECTORIO\n");
        exit(1);
  }
  if ((fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0){
        fprintf(stderr, "ERROR AL ABRIR EL FICHERO DE SALIDA\n");
        exit(1);
  }
  while ((ent = readdir(d)) != NULL){
    if (stat(ent->d_name, &st) != 0){
            perror("ERROR STAT");
            exit(1);
    }
    if(S_ISREG(st.st_mode)){
      sprintf(ruta,"%s/%s",argv[1],ent->d_name);
      write(fd, ruta, strlen(ruta));
      write(fd, "\n", sizeof(char));
    }

	}
  printf("HEMOS ACABADO");
}
