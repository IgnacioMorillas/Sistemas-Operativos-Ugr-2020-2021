#define _POSIX_C_SOURCE 1
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

#define tamano 1024
#define longnombre 50
int hijos_acabados=0;

struct mensaje{
	pid_t pid;
	int id1, id2;
	char operacion;
};

static void manejador() {
	int estado;

	int pid = wait(&estado);
 
    printf("\n(SISTEMA)-> FINALIZADO EL HIJO %d, resultado de la operacion %d \n\n",pid,WEXITSTATUS(estado));
	hijos_acabados++;

}

int main(int argc,char* argv[]){
	struct sigaction sa;
/*	if(setvbuf(stdout,NULL,_IONBF,0)){
		perror("error aqui");
	}*/
	sa.sa_handler = manejador;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(sigaction(SIGCHLD, &sa,NULL)<0){
		perror("Lanzador: Error en crear sigactin");
		exit(EXIT_FAILURE);
	}

	if(argc!=3){
		perror("Lanzador: Error en número de argumentos");
		exit(EXIT_FAILURE);
	}
	
	char *ptr;
	int n_clientes = strtol(argv[1],&ptr,10);
	
	if(mkfifo("FIFOpet",0666)<0){
		perror("Lanzador: Error creando FIFOpet");
		exit(EXIT_FAILURE);
	}
	
	int fdFIFOpet;

	if((fdFIFOpet=open("FIFOpet",O_RDWR))<0){
		perror("Lanzador: Error abrino FIFOpet");
		exit(EXIT_FAILURE);
	}



	pid_t pid;
	for(int i=0; i < n_clientes;i++){
		if((pid=fork())<0){
			perror("Lanzador: Error creando hijo");
			exit(EXIT_FAILURE);
		}
		else{
			if(pid==0){
				srand(getpid());
				int numero_rand1 = rand() % 30 + 1;
				int numero_rand2 = rand() % 30 + 1;
				int numero_rand3 = rand() % 2 +1;

				struct mensaje peticion;
				peticion.pid=getpid();

				if(numero_rand3==1){
					peticion.operacion='+';
				}
				else{
					peticion.operacion='-';
				}

				DIR *dir;
				struct dirent *ent;
				struct stat st_aux;
				
				if((dir=opendir(argv[2]))==NULL){
					perror("/-CLIENTE-/: Error al abrir el directorio");
					exit(EXIT_FAILURE);
				}

				int contador=1;

				while((ent=readdir(dir))!=NULL){
					char ruta[tamano];
					sprintf(ruta,"%s/%s", argv[2], ent->d_name);
					if(stat(ruta,&st_aux)<0){
						perror("/-CLIENTE-/: Error en abrir stat");
						exit(EXIT_FAILURE);//EN VERDA SI QUITAS ESTO EL PROGRAMA CONTINUALEYEN OTRO, Depende de loq ue querarmos
					}
					if(S_ISREG(st_aux.st_mode)){
						if(contador == numero_rand1 || contador == 1){
							peticion.id1 = contador;
						}
						if(contador == numero_rand2 || contador == 2){
							peticion.id2 = contador;
						}
						contador++;
					}
				}
				//printf("%d %d %d %c\n",peticion.pid,peticion.id1,peticion.id2,peticion.operacion);		
				char nombre_fifoCliente[longnombre];
				sprintf(nombre_fifoCliente,"FIFO.%d",getpid());

				if(mkfifo(nombre_fifoCliente,0666)<0){
					perror("/-CLIENTE-/: Error creando fifo.PID");
					exit(EXIT_FAILURE);
				}
			
				int fdFIFOusu;
				if((fdFIFOusu=open(nombre_fifoCliente,O_RDWR))<0){
					perror("/-CLIENTE-/: Error abriendo fifocliente");
					exit(EXIT_FAILURE);
				}
				

				if(write(fdFIFOpet,&peticion,sizeof(struct mensaje))<0){
					perror("/-CLIENTE-/: Error escribiendo peticion");
					exit(EXIT_FAILURE);
				}
				else{
					printf("/-CLIENTE-/: SOLICITUD enviada con exito\n");
				}
							//printf("antes de leer");
				char buffrespuesta[tamano];
				int totalleido;
				if((totalleido=read(fdFIFOusu,buffrespuesta,sizeof(char)*tamano))<0){
					perror("/-Cliente-/: Error en lectura de respuesta");
					exit(EXIT_FAILURE);
				}
				printf("/-CLIENTE-/:RESPUESTA DEL SERVIDOR : %s\n",buffrespuesta);
				close(fdFIFOusu);
				unlink(nombre_fifoCliente);
				exit(0);
			}
		}
	}

	pid_t pid_servidor;
	if((pid_servidor=fork())<0){
		perror("LaANZADOR: Error creando srvidor");
		exit(EXIT_FAILURE);
	}
	else{
		if(pid_servidor==0){
			//basicamenteCargamos en la entrada el descriptor de FIFOPET
			dup2(fdFIFOpet,STDIN_FILENO);
			execl("./servidor","./servidor",argv[2],NULL);
			perror("No se ha podido crear el hijo servidor");
			exit(EXIT_FAILURE);
		}
	}
	if(pid!=0){
		while(hijos_acabados<n_clientes);
		//kill(pid_servidor, SIGKILL);
		close(fdFIFOpet);
		unlink("FIFOpet");
		printf("******************LANZADOR TERMINA*******************\n");
		exit(0);			
	}
	//printf("MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAL\n");
//	exit(0);
}
