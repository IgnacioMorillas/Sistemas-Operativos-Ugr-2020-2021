/*
Servidor:
  - crea un FIFO
  - Abre el FIFO y espera que alguien escriba un PID
  - Crea un hijo (y este crea un FIFO con nombre "cliente.<PID>")(FUNCION DEL HIJO EN VERDAD)
  - El servidor se asegura que no queden hijos zombies

Criado:
  - Crea un fifo llamado "cliente.<PID>"
  - Envia la señal sig_USR1 al PID
  - Espera que escriba un número y lo muestrapor pantalla

Cliente:
  - Abre el fifo y envia su PID
  - Se bloquea hasta que llegue la señal sig_USR1
  - Escribe el argv[1] en el fifo y termina
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void manejador(int s) {
    wait(NULL);
}

int main() {
    char c;
    int fd, fdHijo;
    pid_t pidLeido, pid;
    char nombreFifo[50];
    struct sigaction sa;
    sa.sa_handler = manejador;//ACCION
    sa.sa_flags = 0;		//DOSE

	//SIGCHLD cuando un hijo está PARADO o termina activa esto
    if(sigaction(SIGCHLD, &sa, NULL) < 0){ 
		perror("Error sigaction");
		exit(-1);
	}
// CREAMOS UN FICHERO FIFO CON PERMISOS DE ESCRITURA Y LECTURA PARA USUARIO GRUPO Y OTROS
    if(mkfifo("FIFO", 0666) < 0){ 
		perror("Error creando fifo");
 		exit(-1);
	}
//ABRE SOLO EN MODO LECTURA
    if((fd = open("FIFO", O_RDWR)) < 0){ 
		perror("Error abriendo fifo"); 
		exit(-1);
	}
//leer el FIFO ya que fd contiene el descriptor de fichero
    while(read(fd, &pidLeido, sizeof(pidLeido)) > 0) {
		//CREO UN HIJO
        if((pid = fork()) < 0){
 			perror("Error fork");
 			exit(-1);
		}
        else if(pid == 0) {
			//
            sprintf(nombreFifo, "Cliente.%d", pidLeido);
            printf("Creado un hijo para atender a %s\n", nombreFifo);
            if(mkfifo(nombreFifo, 0666) < 0){ 
				perror("Error creando fifo"); 
				exit(-1);
			}
            if((fd = open(nombreFifo, O_RDWR)) < 0){ 
				perror("Error abriendo fifo");
 				exit(-1);
			}
            printf("Enviando la señal %d a %d\n", pidLeido, SIGUSR1);
            if(kill(pidLeido, SIGUSR1) < 0){
 				perror("Error kill");
				exit(-1);
			}
            while(read(fd, &c, sizeof(char)) > 0) {
                printf("%c", c);
            }
            unlink(nombreFifo);
            exit(0);
        }
    }
}
