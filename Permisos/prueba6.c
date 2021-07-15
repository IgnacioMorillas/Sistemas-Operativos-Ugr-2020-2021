/*

Añadir permisos de escritura a "otros" a todos los ficheros regulares de un directorio pasado por parametro.
	
b)Si tiene que se lo quite, y si no, que se lo ponga

*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char*argv[]){
	if(argc !=2){
		perror("NÚMERO DE ARGUMENTOS INCORRECTO");
		exit(-1);
	}
	
	DIR * dir;
	struct dirent *ent;
	struct stat st;
	char ruta[512];

	if((dir=opendir(argv[1]))<0){
		perror("ERROR EN EL DIRECTORIO");
		exit(-1);
	}

	mode_t permiso;

	while((ent = readdir(dir))!=NULL){
		sprintf(ruta,"%s/%s",argv[1],ent->d_name);
		if(stat(ruta,&st)<0){
			perror("ERROR AL RECUPERAR INFO");
			exit(-1);
		}
		// VEMOS SI ES REGULAR

		if(S_ISREG(st.st_mode)){
		// VEMOS SI TIENE EL PERMISO, USAMOS LAS OPERACIONES AND Y OR PARA VERLO, EL PARENTESIS ES SUPER IMPORTANTE, si no se pone te lo compara mal.
			if(((st.st_mode & S_IWOTH) == S_IWOTH) || ((st.st_mode & S_IROTH) == S_IROTH)){
				//printf("%d  ",(st.st_mode & S_IWOTH));
				//printf("%d  ",(S_IWOTH));
				//printf("%d   ",(st.st_mode & S_IWOTH == S_IWOTH));
				//printf("%d   ",(st.st_mode & S_IROTH == S_IROTH));
				//printf("HOLA");

//QUITA PRIVILEGIOS
				permiso= st.st_mode & ~S_IWOTH & ~S_IROTH;
			}
//EL ELSE PARA HACER LA INVERSA
			else{
				//SUMA PRIVILEGIOS
				permiso= st.st_mode | S_IWOTH | S_IROTH;
			}
			printf("%d\n",permiso);
			chmod(ruta,permiso);
		}
	}
	closedir(dir);
}
