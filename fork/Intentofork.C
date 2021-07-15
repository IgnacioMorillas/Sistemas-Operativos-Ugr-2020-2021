/*

Proceso que recibe un path como argument en la linea de ordenes y
crea un hijo que comprobará si un fichero regular, con permis de
 lectura para otros y mostrará por pantalla si lo es.
-El padre esperara a que el hijo termine y mstrará un error si el hijo no ha finalizado corectamente
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
		perror("NUMERO DE ARGUMENTOS INCORRECTO");
		exit(-1);
	}

	pid_t pid;
	
	if((pid=fork())<0){
		perror("NO SE PUDO CREAR EL HIJO");
		exit(-1);
	}
	//ACCIONES PARA EL HIJO
	else{
		if(pid==0){
			struct stat st;
			if(stat(argv[1],&st)<0){
				perror("NO SE PUDO ABRIR EL FICHERO");
				exit(-1);
			}
			if(S_ISREG(st.st_mode) && ((st.st_mode & S_IROTH) == S_IROTH)){
				printf(":)");
				exit(0);
			}
			else{
				printf(":(");
				exit(-1);
			}
		}
		//ACCIONES PADRE
		else{
			int ret;
			wait(&ret);
			if(WEXITSTATUS(ret)==0){
				printf("Finaliz:%d\n", WEXITSTATUS(ret));
				printf("ES REGUAR DICE EL HIJO\n");
			}
			else{
				printf("Finaliz:%d\n", WEXITSTATUS(ret));
				printf("NO ES REGUAR DICE EL HIJO\n");
			}
		}		
	}
}
