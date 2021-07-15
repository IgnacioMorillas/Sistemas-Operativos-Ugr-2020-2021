//EJERCICIO
/*
Crear un programa que reciba como argumento un directorio, El proceso
creará 2 proceso hijos
- El primer hijo leerá todas las entradas del directorio indicado como parametro.
	Escribirá mediante un pipe el nombre de aquellas entradas que sean un fichero regular.
- El segundo hijo leerá del cauce el nombre de todas las entradas enviadas por el primer hijo y las mostrará por pantalla.
- El padre esperará a que ambos hijos terminen y mostrará un mensaje en caso de que alguno de ellos finalice con error.
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
	if(argc !=2){
		perror("Numero de argumentos incorreto\n");
		exit(-1);
	}
	int fd_pipe[2];
	char ruta[512];
	struct stat st;
	DIR *d;
	struct dirent *end;
	pid_t pid;
	ino_t inodo;
	if(pipe(fd_pipe)<0){
		perror("ERROR CON EL PIPE");
		exit(-1);
	}
	if((pid = fork())<0){
		perror("No se pudo crear el hijo");
		exit(-1);
	}
	else{
		if(pid==0){
			//HIJO 1
			close(fd_pipe[0]);
			if((d=opendir("."))==NULL){
				perror("ERROR EN ABRIR EL DIRECTORIO");
				exit(-1);
			}
			while((end=readdir(d))!=NULL){
				sprintf(ruta,"%s/%s",argv[1],end->d_name);
				if(stat(ruta,&st)<0){
					perror("ERROR E OBTENER EL STAT");
					exit(-1);
				}
				if(S_ISREG(st.st_mode)){
					if(write(fd_pipe[1],&end->d_ino, sizeof(ino_t))<0){
						perror("ERROR EN ESCRIBIR EN EL FD1");
						exit(-1);
					}
				}
			}
			close(fd_pipe[1]);
			exit(0);
		}
	}
	if((pid = fork())<0){
		perror("No se pudo crear el hijo");
		exit(-1);
	}
	else{
		//HIJO2
		if(pid==0){
			close(fd_pipe[1]);
			while(read(fd_pipe[0],&inodo, sizeof(ino_t))>0){
				printf("inodo recibido: %lu\n",inodo);
			}
			close(fd_pipe[0]);
			exit(0);
		}
		else{
			//PAPA
			int ret;
   			wait(&ret);
	      	if(WEXITSTATUS(ret!=0)){
       			printf("Mi hijo1 tuvo problemas...\n");
   			}
			else{
				printf("Hijo 1 finalizado correctamente\n");
				close(fd_pipe[1]);
//ESTOS CLOSES SON IMPORTANTES YA QUE HASTA QUE EL PADRE TENGA ABIERTO SUS CANALES, EL QUE ESTA ESPERANDO PARA ESCUCHAR NO FINALIZA LA ESPERA
			}
			wait(&ret);
	      	if(WEXITSTATUS(ret)!=0){
       			printf("Mi hijo2 tuvo problemas...\n");
   			}
			else{
				printf("Hijo 2 finalizado correctamente\n");
				close(fd_pipe[0]);
			}
			printf("FIN PAPA\n");
		}
	}
}

