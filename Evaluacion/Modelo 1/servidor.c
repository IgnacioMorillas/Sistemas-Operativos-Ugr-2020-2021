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

#define tamano 1024
#define longnombre 50

struct mensaje{
	pid_t pid;
	int id1, id2;
	char operacion;
};

int main(int argc, char* argv[2]){
	if(argc != 2){
		perror("**SERVIDOR**: Error en número de argumentos");
		exit(-1);
	}
	//int Fifopet;
	//Creo que está ya abierto
	//Fifopet=STDIN_FILENO;
	//close(STDIN_FILENO);

	/*if((Fifopet = open("FIFOpet",O_RDWR))<0){
		perror("ERROR ABRIENDO FIFO1");
		exit(-1);
	}*/
	//printf("HOLAAA\n");
	int Bleidos=0;
	struct mensaje msg;
	//printf("----->%d\n",(Bleidos=read(STDIN_FILENO,&msg,sizeof(struct mensaje))));
	while((Bleidos=read(STDIN_FILENO,&msg,sizeof(struct mensaje)))>0){
//printf("---------------------------------\n");
		DIR* dir;
  		struct dirent *ent;
  		struct stat st_aux;
	printf("%d %d %d %c\n",msg.pid,msg.id1,msg.id2,msg.operacion);

		if((dir=opendir(argv[1]))==NULL){
			perror("**SERVIDOR**: Error al abrir el directorio");
			//exit(-1);
		}
//printf("---------------------------------\n");
		int contador=1;
		off_t tama1;
		off_t tama2;
		while((ent=readdir(dir))!=NULL){
			char ruta[tamano];
			sprintf(ruta,"%s/%s", argv[1], ent->d_name);
			if(stat(ruta,&st_aux)<0){
				perror("**SERVIDOR**:Error en el stat");
				//exit(-1);
			}
			else{
				if(S_ISREG(st_aux.st_mode)){
					//break;
					if(contador==msg.id1){
						tama1=st_aux.st_size;
						//printf("tama 1 = %ld, st = %ld",tama1,st_aux.st_size);
					}
					if(contador==msg.id2){
						tama2=st_aux.st_size;
				//printf("tama 2 = %ld, st = %ld\n",tama2,st_aux.st_size);
					}
					contador++;
				}

			}
		}
//printf("---------------------------------\n");
		int resultado=0;
		if(msg.operacion=='+'){
			//SUMA
			resultado= tama1+tama2;
		}
		else{
			//RESTA
			resultado= tama1-tama2;
		}
		int fdFIFOusu;
		char nombre_fifoCliente[longnombre];
		sprintf(nombre_fifoCliente,"FIFO.%d",msg.pid);
		if((fdFIFOusu=open(nombre_fifoCliente, O_WRONLY))<0){
			perror("**SERVIDOR**:ERROR ABRIENDO FIFO2");
			//exit(-1);
		}
//printf("---------------------------------\n");
		char buffrespuesta[tamano];
		sprintf(buffrespuesta,"Cliente %d: %ld %c %ld = %d",msg.pid,tama1,msg.operacion,tama2,resultado);
		//printf("---------->%s\n",buffrespuesta);
	//if(write(fdFIFOusu,buffrespuesta,strlen(buffrespuesta)*sizeof(char)+1)!=strlen(buffrespuesta)*sizeof(char)+1){
	if(write(fdFIFOusu,buffrespuesta,strlen(buffrespuesta)*sizeof(char)+1)!=strlen(buffrespuesta)*sizeof(char)+1){
			perror("**SERVIDOR**: Error a enviar resultado");
			//exit(-1);
		}
		close(fdFIFOusu);
	}
	//close(Fifopet);
	exit(0);	
}
