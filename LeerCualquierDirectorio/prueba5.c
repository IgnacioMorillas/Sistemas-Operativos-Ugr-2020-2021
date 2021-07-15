//Mostrar los ficheros por pantalla con tamaño mayor a 50 bytes de un directorio cualquiera

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main( int argc, char* argv[]){
	if(argc != 2){
		perror("NUMERO DE ARGUMENTOS INCORRECTO");
		exit(-1);
	}

	DIR * dir;
	struct dirent *ent;
	struct stat st;
	char ruta[512];
	
	if((dir=opendir(argv[1]))==NULL){
		perror("ERROR AL ABRIR EL DIRECTORIO");
	}

	while((ent=readdir(dir))!=NULL){
		//GUARDAMOS RUTA
		sprintf(ruta,"%s/%s", argv[1], ent->d_name);
		//printf("%s\n",ruta);

		if(stat(ruta, &st) < 0){
			perror("ERROR STAT");
			exit(-1);
		}
		if(st.st_size>50){
			printf("%s\n", ent->d_name);
		}
	}
	closedir(dir);
}
