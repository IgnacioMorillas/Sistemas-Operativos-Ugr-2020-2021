/*
EXEC-> Sirve para ejecutar un programa que ya existe en disco,
dentro del programa, cuando se ejecuta se elimina el proceso actual
y pasa a ser el principal el nuevo. 
Cuando exec da error es el unico caso en el que el programa anteriror sigue ejecutandose.
//exec una familia por tanto;
	-l -> list(es el que vamos a usar)
	-v -> vector
Basicamente como le pasamos los argumentos al programa que vamos a ejecutar, como lista o vector, por tanto hay que eleegir
	-p -> path ( PATH=/bin:/usr/bin: dir:dir:dir
	EJ: $PATH =
		$grep /etc/passwd
Path rutas donde se encuentran los ejecutables, solo funcionan los
comandos macros. poreso lo anterior no funciona. (ls,grep,etc)

execl("./ejemplo","./ejemplo","dirA",,"ficj.txt","34",NULL);
("nombre del ejecutable", parametros(como minimo se le pasa 1 que va a ser el nombbre del programa.
Se va añadiendo con comas, para indicar que no le queda mas parametros se acaba con NULL.

*/
/*
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(){
	int fd;
	if((fd=open("a.txt",O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR))<0){
		perror("....W");
	}
	dup2(fd,STDOUT_FILENO);
	printf("HOLA FRANCHU:) \n");
	close(fd);
}
*/

/*
TDF
0 -> STDIN -> teclado
1 -> STDOUT -> patalla
2 -> STDERR -> pantalla
3 -> fd -> a.txt
Lo que hace es el 1 cargarlo en el a.txt por loq ue todos los printf(que se mostrarian por pantalla) se iran al fichero


/*

fd[1] lee
fd[0] escribe
cierran comunicacion con el padre que crea 2 hijos Hijo 2 lee y hijo 1 escribe en hijo 2
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

int main(){
	int fd[2];
	int pid;
	if(pipe(fd)<0){
		perror("...");
		exit(-1);
	}
	if((pid=fork())<0){
		perror("_");
		exit(-1);
	}
	else if(pid==0){
		close(fd[0]);
		if(dup2(fd[1],STDOUT_FILENO)<0){
			perror("___");
			exit(-1);
		}
		execlp("grep","grep","root","/etc/passwd",NULL);
		perror("__");
		exit(-1);
	}
	if((pid=fork())<0){
		perror("_");
		exit(-1);
	}
	else if(pid==0){
		close(fd[1]);
		if(dup2(fd[0],STDIN_FILENO)<0){
			perror("...");
			exit(1);
		}
		execlp("cut","cut","-d:","-f1",NULL);
		perror("___");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);
	printf("--------\n");
}
}

