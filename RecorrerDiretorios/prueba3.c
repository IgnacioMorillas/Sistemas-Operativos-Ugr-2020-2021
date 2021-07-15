//Mostrar el contenido del directorio actual y guardarlo en un fichero
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main( int argc, char *argv[]){
	
	//COMPROBAMOS QUE SE HAN INTRODUCIDO EL NUMEROA DECUADO DE ARGUMENTOS
	if(argc =! 2){
		fprintf(stderr, "ERROR EN EL NÚMERO DE ARGUMENTOS\n");
		exit(-1);
	}

	DIR *dir; //Puntero a diectorio
	int fd;// identidicador de fichero
	struct dirent * ent; //structura del directorio
	char buffer[250];

	//abrimos el directorio que vamos a copiar
	if((dir = opendir("."))==NULL){
		perror("ERROR AL ABRIR EL DIRECTORIO");
		exit(-1);
	}

	//Abrimos el fichero a escribir, si no existe lo crea
	if((fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR))<0){
		perror("ERROR AL ABRIR EL FICHERO DE SALIDA");
		exit(-2);
	}
	
	//Leemos el puntero del directorio el cual pasara al siguiente en la siguiente lectura
	while((ent = readdir(dir)) != NULL){
		//El strlen devuelve el tamaño de string sin el \0
		
		write(fd,ent->d_name, strlen(ent->d_name));
		write(fd,"\n", sizeof(char));
	}
	closedir(dir);
    close(fd);
}
