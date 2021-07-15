#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>


int main(int argc,char* argv[]){

	if(argc!=2){
		perror("/-Buscardor-/: Error en número de argumentos");
		exit(EXIT_FAILURE);
	}
	int inodobuscado;
	int Bleidos;
	while(1){
	while((Bleidos = read(STDIN_FILENO,&inodobuscado,sizeof(int)))>0){
		printf("TRABAJO\n");
		if(inodobuscado!=0){
			DIR* dir;
  			struct dirent *ent;
  			struct stat st_aux;

			if((dir=opendir(argv[1]))==NULL){
				perror("/-Buscardor-/: Error al abrir el directorio");
				exit(EXIT_FAILURE);
			}
			while((ent=readdir(dir))!=NULL){
				char ruta[1024];
				sprintf(ruta,"%s/%s", argv[1], ent->d_name);
				if(stat(ruta,&st_aux)<0){
					perror("/-Buscardor-/:Error en el stat");
					exit(-1);
				}
				else{
					printf("TRABAJO\n");
					int inodolocal = (int) st_aux.st_ino;
					if(inodolocal==inodobuscado){
						printf("Nombre : %s, numero_inodo: %ld, numero_enlaces_duros: %ld, permisos del propietario: %d\n",ent->d_name,st_aux.st_ino,st_aux.st_nlink,st_aux.st_mode);
					}
				}	
			}
		}
		else{
			printf("Inodo con valor 0, FINALIZAMOS\n");
			exit(0);
		}	
	}
	}
	exit(0);
}
