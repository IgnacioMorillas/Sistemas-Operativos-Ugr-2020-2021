/*
Maestro: Se le pasa por parametro el intervalo de numeros para calcular los numeros primos
Creará 2 esclavos, y dividirá ese intervalo en 2
2 cauces sin nombre, y ira mostrando los resultados en orden
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
	if(argc !=3){
		perror("ERROR EN NUMERO DE PARAMETROS");
		exit(-1);
	}
	int fd_pipe1[2];
	int fd_pipe2[2];

	pid_t pid;
	
	if(pipe(fd_pipe1)<0){
		perror("ERROR EN CREACION DE PIPE1");
		exit(-1);
	}
	if(pipe(fd_pipe2)<0){
		perror("ERROR EN CREACION DE PIPE2");
		exit(-1);
	}
//HIJO 1
	if((pid=fork())<0){
		perror("ERROR EN CREACION DE HIJO1");
		exit(-1);
	}
	else{
		if(pid==0){
			close(fd_pipe1[0]);
			close(fd_pipe2[0]);
			close(fd_pipe2[1]);
			int numeroaux=((atoi(argv[2])-atoi(argv[1]))/2);
			char n_medio_string[16];
			sprintf(n_medio_string,"%d",(numeroaux+atoi(argv[1])));
			//printf("%s",n_medio_string);
			if(dup2(fd_pipe1[1],STDOUT_FILENO)<0){
				perror("error en dup2.1");
				exit(-1);
			}
			
			execl("./esclavo","./esclavo", argv[1],n_medio_string, NULL);
			//apartir de aqui solo ejecutará si falla el execl
			perror("error en el execl hijo1");
			exit(-1);
		}	
	}
//HIJO2
	if((pid=fork())<0){
		perror("ERROR EN CREACION DE HIJO1");
		exit(-1);
	}
	else{
		if(pid==0){
			close(fd_pipe2[0]);
			close(fd_pipe1[0]);
			close(fd_pipe1[1]);

			if(dup2(fd_pipe2[1],STDOUT_FILENO)<0){
				perror("error en dup2.2");
				exit(-1);
			}
			
			int numeroaux=((atoi(argv[2])-atoi(argv[1]))/2);
			char n_medio_string[16];
			sprintf(n_medio_string,"%d",numeroaux+atoi(argv[1])+1);
			
			execl("./esclavo","./esclavo",n_medio_string,argv[2],NULL);
			perror("error en el execl hijo2");
			exit(-1);
		}
	}	


	//PAPA
	if(pid!=0){
		int ret;
		char c;
		close(fd_pipe1[1]);
		close(fd_pipe2[1]);
       	printf("Resultado hijo1\n");
		while(read(fd_pipe1[0],&c,sizeof(char))>0){
			printf("%c",c);			
		}
		close(fd_pipe1[0]);
		printf("resultado hijo2\n");
		while(read(fd_pipe2[0],&c,sizeof(char))>0){
			printf("%c",c);			
		}
		close(fd_pipe2[0]);
		wait(&ret);
		if(WEXITSTATUS(ret)!=0){
       		printf("Mi hijo 1 tuvo problemas...\n");
   		}
		else{
			printf("Mi hijo 1 finalizo bien\n");
		}
		wait(&ret);
		if(WEXITSTATUS(ret)!=0){
       		printf("Mi hijo 2 tuvo problemas...\n");
   		}
		else{
			printf("Mi hijo 2 finalizo bien\n");
		}
	}
}
