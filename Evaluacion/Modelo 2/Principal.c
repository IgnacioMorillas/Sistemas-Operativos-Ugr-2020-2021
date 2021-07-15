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
	if(argc<3){
		perror("/*Principal*/: Error en número de argumentos");
		exit(EXIT_FAILURE);
	}
	int fd_cauce1[2];
	if(pipe(fd_cauce1)<0){
		perror("/*Principal*/: ERROR CON EL PIPE");
		exit(EXIT_FAILURE);
	}

	int num_hijos;
	num_hijos=strtol(argv[2],NULL,10);

	pid_t pid;
	for(int i;i<num_hijos;i++){
		if((pid=fork())<0){
			perror("/*Principal*/: Error creando hijo");
			exit(EXIT_FAILURE);
		}
		else{
			if(pid==0){
				close(fd_cauce1[1]);
				//while(read(fd_pipe[0],&inodo, sizeof(ino_t))>0){
			//		printf("inodo recibido: %lu\n",inodo);
				//}
				dup2(fd_cauce1[0],STDIN_FILENO);
				execl("./buscar","./buscar",argv[1],NULL);
				perror("/*Principal*/: Error en exec");
				exit(EXIT_FAILURE);
			}
		}
	}
	close(fd_cauce1[0]);
	//DIR* dir;
  	//struct dirent *ent;
  	struct stat st_aux;
	int contador=3;
	while(argv[contador]!=NULL){
		/*if((dir=opendir(argv[1]))==NULL){
			perror("Principal: Error al abrir el directorio");
			//exit(EXIT_FAILURE);
		//}
		//while((ent=readdir(dir))!=NULL){
		*/
			char ruta[1024];
			sprintf(ruta,"%s/%s", argv[1], argv[contador]);
			if(stat(ruta,&st_aux)<0){
				perror("/*Principal*/: Error en el stat");
				exit(-1);
			}
			else{
					printf("TRABAJO\n");
				int inodo = (int) st_aux.st_ino;
				if(write(fd_cauce1[1],&inodo,sizeof(int))<0){
					perror("/*Principal*/:  Error escribiendo peticion");
					exit(EXIT_FAILURE);
				}
				else{
					printf("%ld\n",st_aux.st_ino);
				}
			}	
		//}	
		contador++;
	}
	printf("LLEGO AQUI\n");
	for(int i=0;i< contador-3;i++){
		if(write(fd_cauce1[1],0,sizeof(ino_t))<0){
			perror("/*Principal*/: Error escribiendo peticion");
			exit(EXIT_FAILURE);
		}
	}

	for(int i;i< num_hijos;i++){
		int estado;
		int pid_finalizado = wait(&estado);
		printf("\n(SISTEMA)-> FINALIZADO EL HIJO %d, resultado de la operacion %d \n\n",pid_finalizado,WEXITSTATUS(estado));	
	}

	printf("\n******HAN TERMINADO TODOS LOS HIJOS*****\n");
	exit(0);

}
