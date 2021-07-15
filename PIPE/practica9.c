//PROGRAMA
/*
 Programa que recibe como argumento la ruta de un directorio, el proceso creara un hijo que se encargara de comprobar
si hay ficheros regulares en el directorio pasado como parametro,
en caso de haberlos el hijo enviara a traves de un pipe el inodo de esos ficheros

El padre mostrara por pantalla todos los inodos escrito por el hijo, y una vez que ste termine(hijo), el padre mostrará un mensaje por pantalla

INODO-> un identificador del fichero
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

int main(int argc, char* argv[]){
	if(argc != 2){
		perror("Error en el número de parametros");
		exit(-1);
	}
	int fd_pipe[2];	// -> Para el canal de comunicación
	char ruta[512];	// -> Contendrá la ruta del fichero
	struct stat st;	// -> Contenedor de la estrucutra del fichero
	DIR * dir; 	   	// -> La ID del directorio que se va a recorrer
	struct dirent * end; //-> Fichero actual en el directoro
	pid_t pid;		// -> Identificador del proceso, padre/hijo
	ino_t inodo;	// -> inodo(identificador unico del fichero)

	//Creamos el pipe
	if(pipe(fd_pipe)<0){
		perror("Error al crear PIPE");
		exit(-1);
	}
	//CREAMOS EL HIJO
	if((pid= fork())<0){
		perror("Error al crear el hijo");
		exit(-1);
	}
	else{
		if(pid==0){
			//PARA EL HIJO
			//ABRIMOS EL DIRECTORIO
			if((dir=opendir(argv[1]))<0){
				perror("ERROR AL ABRIR EL DIRECTORIO");
				exit(-1);
			}

			//ESTABLECEMOS EL CANAL DE COMUNICACION
			close(fd_pipe[0]);

			//RECORREMOS EL DIRECTORIO
			while(( end = readdir(dir))!=NULL){
				//ESTABLECEMOS RUTA
				sprintf(ruta,"%s/%s",argv[1],end->d_name);
				//recogemos la estructura
				if(stat(ruta,&st)<0){
					perror("ERROR EN EL STAT");
					exit(-1);
				}
				//VEmos si es regular
				if(S_ISREG(st.st_mode)){
					//Escribimos en el canal de comunicacion
					if(write(fd_pipe[1],&end->d_ino,sizeof(ino_t))<0){
						perror("ERROR AL ESCRIBIR EN EL PIPE");
						exit(-1);
					}
				}
			}
			//ESTABLECEMOS EL CANAL DE COMUNICACION(NO ENTIENDO BIEN)
			close(fd_pipe[1]);
			exit(0);
		}
		else{
			//CERRAMOS EL CANAL DE COMUNICACION CON EL HIJO PARA SOLO ESCUCHAR
			close(fd_pipe[1]);
			while(read(fd_pipe[0],&inodo, sizeof(ino_t))>0){

				printf("inodo recibido: %lu\n",inodo);
			}
			close(fd_pipe[0]);
			//ESPERAMOS A QUE EL HJO TRMINE PARA CERRAR.
			wait(NULL);
			printf("FIN\n");
		}
	}

}
