//gcc -o esclavo esclavo.c -lm
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		//perror("Error en argumentos");
		exit(-1);
	}
	int inicio = atoi (argv[1]);
	int fin = atoi(argv[2]);
	int primo;
	//int fd_pipe[2];
	/*if(pipe(fd_pipe)<0){
		perror("Error con el pipe");
		exit(-1);
	}*/
	for(int i = inicio;i<=fin;i++){
		primo=i;
		for(int k=2; k <= sqrt(i) && primo ;k++){
			if(i % k==0){
				primo =0;
			}
		}
		if(primo){
			printf("%d\n",i);
		}
	}
	exit(0);
}
