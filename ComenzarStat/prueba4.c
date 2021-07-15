//Mostrar el contenido del directorio actual y guardarlo en un fichero 
//si su tamaño es mayor a 50 bytes

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc =! 2){
		perror("ERROR EN EL NÚMERO DE ARGUMENTOS");
		exit(-1);
	}

	DIR * dir;
	int fd;
	struct dirent *ent;

	struct stat st;//Contiene la estructura del fichero

	//abrimos el directorio
	if((dir = opendir("."))==NULL){
		perror("ERROR AL ABRIR EL DIRECTORIO");
		exit(-2);
	}

	//Abrimos el fichero
	if((fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR))<0){
		perror("ERROR A ABRIR FICHERO");
		exit(-3);
	}

	while((ent = readdir(dir))!=NULL){
		//CALCULAMOS EL VALOR DEL FICHERO ACTUAL
		if(stat(ent->d_name, &st) !=0){
			perror("ERROR OBTENIENDO EL STAT");
			exit(-4);
		}

		//EN EL ESQUEMA STAT VEMOS QUE SIZE NOS DICE EL TAMAÑO EN BYTE
		if(st.st_size > 50){
			write(fd, ent->d_name, strlen(ent->d_name));
			write(fd, "\n", sizeof(char));
		}
	}

	closedir(dir);
	close(fd);
}
