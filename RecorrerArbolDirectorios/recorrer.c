#define _DEFAULT_SOURCE 
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

void Recorrer(char* argv){
	DIR * dir;
	struct dirent *ent;
//printf("QUE LLEGA --> %s\n",argv);
	if((dir = opendir(argv))==NULL){
		perror("Error al abrir directorio");
	}
	while((ent = readdir(dir))!=NULL){
		char ruta[512];
		sprintf(ruta,"%s/%s",argv,ent->d_name);
		if(ent->d_type ==DT_DIR && (0!=strcmp(ent->d_name,"..")&& 0!=strcmp(ent->d_name,"."))){
			printf("directorio -> %s\n",ent->d_name);
			Recorrer(ruta);
		}
		else{
			if((0!=strcmp(ent->d_name,"..")&& 0!=strcmp(ent->d_name,"."))){
				printf("fichero -> %s\n",ent->d_name);
			}
		}
	}
}

int main(int argc,char* argv[]){
	if(argc!=2){
		perror("ERROR EN NUMERO ARGUMENTOS");
		exit(-1);
	}

	Recorrer(argv[1]);

	return 0;
}
