#define _DEFAULT_SOURCE
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
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		perror("Error en número de argumentos");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid;
	pid_t hijos[6];
	
	for(int i=0; i< 6;i++){
		if((pid=fork())<0){
			perror("error creando hijo");
			exit(EXIT_FAILURE);
		}
		else{
			hijos[i]=pid;
			if(pid==0){
				exit(0);
			}
		}
	}

	for(int i=0;i<6;i++){
		int aux;
		int estado;
		aux=i%2;
		if(aux!=0){
			pid = waitpid(hijos[i],&estado,WNOHANG);
			printf("Ha terminado impar el hijo %d con resultado %d\n",pid,WEXITSTATUS(estado));
		}
	}
	for(int i=0;i<6;i++){
		int aux;
		int estado;
		aux=i%2;
		if(aux==0){
			pid = waitpid(hijos[i],&estado,WNOHANG);
			printf("Ha terminado par  el hijo %d con resultado %d\n",pid,WEXITSTATUS(estado));
		}
	}

	exit(0);
}
