/*
Va a recibir un nuevo directorio como argumento, y va a crear 2 hijos
Estos hijos van a ejecutar otro programa que va a recibir como
argumentos el directorio que ha recibido el padre y 2 enteros.
El entero que se le pasa sirve para mostrar por pantala el numero de 
inodo de todos los archivos que sean de un intervalo al numero 
indicado como argumento.

El padre antes dellamar a los hijos, va a comprobar el tamaño mas
pequeña y la mas grande de la entrada, ac ada hijo va coger la mitad 
y ek otro la otra mitad.

El padre se va a comunicar con los hijos po run cauce, y el padre va 
a leer del cauce, el inodo y el tamño y va a comprobar que ese inodo tiene realmente ese tamaño.
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
		perror("ERROR EN NUMERO DE PARAMETROS");
		exit(-1);
	}
	int fd_pipe1[2];
	int fd_pipe2[2];
	if(pipe(fd_pipe1)<0){
		perror("ERROR EN CREACION DE PIPE1");
		exit(-1);
	}
	if(pipe(fd_pipe2)<0){
		perror("ERROR EN CREACION DE PIPE2");
		exit(-1);
	}
	char c;
	pid_t pid;
	DIR * d;
	struct dirent *ent;
	struct stat st;
	//VAMOS A VER EL VALOR MINIMO Y MAXIMO
	int valor_max =-1;
	int valor_minimo=99999;

	if((d=opendir(argv[1]))<0){
		perror("ERROR AL ABRIR EL DIRECTORIO");
		exit(-1);
	}
	while((ent=readdir(d))!=NULL){
		if(stat(ent->d_name,&st) <0){
			perror("ERROR EN EL STAT");
			exit(-1);
		}

		if(st.st_size > valor_max){
			valor_max= st.st_size;
		}
		if(st.st_size< valor_minimo){
			valor_minimo=st.st_size;
		}
	}
	//LO PASAMOS A SRING PARA PODER SER LEIDO POR EXECL
	char max[10];
	char min[10];
	char mitad[10];
	char mitad1[10];
	sprintf(max,"%d",valor_max);
	sprintf(min,"%d",valor_minimo);
	sprintf(mitad,"%d",((valor_max-valor_minimo)/2)+valor_minimo);
	sprintf(mitad1,"%d",((valor_max-valor_minimo)/2)+valor_minimo+1);
	printf(" MIN: %s\n MITAD: %s\n MITAD1: %s\n MAX: %s\n",min,mitad,mitad1,max);

	if((pid=fork())<0){
		perror("ERROR EN CREAR HIJO 1");
		exit(-1);
	}
	if(pid==0){
		printf("HIJO 1\n");
		close(fd_pipe1[0]);
		close(fd_pipe2[0]);
		close(fd_pipe2[1]);
		if(dup2(fd_pipe1[1],STDOUT_FILENO)<0){
			perror("error en dup2.1");
			exit(-1);
		}
		//printf("MENSAJE DE TU HIJO\n");
		execl("./hijo","./hijo",argv[1],min,mitad,NULL);
		perror("ERROR EN EL HIJO1");
		exit(-1);		
	}
	if((pid=fork())<0){
		perror("ERROR EN CREAR HIJO 1");
		exit(-1);
	}
	if(pid==0){
		printf("HIJO 2\n");
		close(fd_pipe2[0]);
		close(fd_pipe1[0]);
		close(fd_pipe1[1]);
		if(dup2(fd_pipe2[1],STDOUT_FILENO)<0){
			perror("error en dup2.2");
			exit(-1);
		}
		//printf("MENSAJE DE TU HIJO\n");
		execl("./hijo","./hijo",argv[1],mitad1,max,NULL);
		perror("ERROR EN EL HIJO1");
		exit(-1);		
	}
	if(pid!=0){
		printf("PAPA\n");
		int ret;
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

		close(fd_pipe1[1]);
		printf("Resultado hijo1\n");
		while(read(fd_pipe1[0],&c,sizeof(char))>0){
			printf("%c",c);			
		}
		//wait(NULL);
		close(fd_pipe2[1]);
		printf("Resultado hijo2\n");
		while(read(fd_pipe2[0],&c,sizeof(char))>0){
			printf("%c",c);			
		}
		exit(0);
	}
	exit(0);
}
