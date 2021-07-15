#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void manejador(int s) {
    printf("Recibia la señal %d\n", s);
}

int main(int argc, char*argv[]) {
    int fdFifo1, fdFifo2;
    char nombreFifo[50];
    pid_t pid;
    struct sigaction sa;
    sa.sa_handler = manejador;
    sa.sa_flags = 0;
//SEÑAL DEFINIDA POR EL USUARIO 1
    if(sigaction(SIGUSR1, &sa, NULL) < 0) {
		perror("Error sigaction");
 		exit(-1);
	}
    if(argc != 2) {
        printf("Error de argumentos\n");
        exit(-1);
    }
    pid = getpid();
//ABRE EL FICHERO FIFO
    if((fdFifo1 = open("FIFO", O_WRONLY)) < 0){
		perror("Error abriendo fifo");
 		exit(-1);
	}
//ESCRIBE EN EL FIFO
    if(write(fdFifo1, &pid, sizeof(pid)) < 0){
 		perror("Error write 1");
		exit(-1);
	}
    sprintf(nombreFifo, "Cliente.%d", pid);
//reemplaza temporalmente la máscara de señal para el proceso con la
// dada por el argumento mask y luego suspende el proceso hasta que
// se recibe una señal
    if(sigsuspend(0) < 0){
    	perror("Error sigsuspend"); 
		exit(-1);
	}
//Abri el otro fichero fifo creado para el
    if((fdFifo2 = open(nombreFifo, O_RDONLY)) < 0){
		perror("Error abriendo fifo");
		exit(-1);
	}    
//LEE la respuesta del servidor
	if(write(fdFifo2, argv[1], sizeof(char)*strlen(argv[1])) < 0){ 
		perror("Error write 2");
		exit(-1);
	}
}
