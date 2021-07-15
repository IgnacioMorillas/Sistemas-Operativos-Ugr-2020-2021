/*
Un procso que recibe un directorio como argumento, vaha crear dos
hijos, que van a ejecutar otro programa que recibe como argumento el
directorio y dos enteros. El entero sirve para mostrar por pantalla
el numero de inodo y el tamaño de todas las entradas del directorio que tenfan un tamaño comprendido entre los numeros pasados como argumento
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include<math.h>

int main(int argc, char* argv[]){
	if(argc != 4){
		perror("ERROR EN EL NUMERO DE ARGUMENTOS");
		exit(-1);
	}
	char *ptr;
	int tama_min=strtol(argv[2], &ptr,10);
	int tama_max=strtol(argv[3], &ptr,10);
	DIR *d;
	struct dirent *ent;
	struct stat st;
	
	if((d=opendir(argv[1]))<0){
		perror("ERROR AL ABRIR DIRECTORIO");
		exit(-1);
	}
	while((ent=readdir(d))!=NULL){
		if(stat(ent->d_name,&st)<0){
			perror("ERROR EN EL STAT");
			exit(-1);
		}
		if(st.st_size<=tama_max && st.st_size>=tama_min){
			printf("INODO: %ld , TAMA: %ld \n", ent->d_ino, st.st_size);
		}
	}
 	closedir(d);
	exit(0);
}
