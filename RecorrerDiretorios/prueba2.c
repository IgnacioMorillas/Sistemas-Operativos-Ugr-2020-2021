//recorre el directrio actual y muestra por pantalla todos los ficheros

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(){
	DIR *dir; // DIR *opendir(const char*); //pa guardar la direccion del directorio a abrit

	struct dirent * ent; //contiene los datos del directorio

	//Abrimos el directorio actual "." El padre sería ".."
	//En caso de error devuelve NULL
	if (( dir = opendir(".")) == NULL){
		perror("ERROR AL ABRIR EL DIRECTORIO");
		exit(-1);
	}
	//lee el puntero donde apunta el directorio, una vez leido pasa al siguiente.
	while ((ent = readdir(dir)) != NULL){
		printf("%s\n", ent->d_name);//%s cadena de caracteres+salto de linea, la cadena se lee de la estructura el atributo d_name
	}
	closedir(dir);//cerramos el flujo de directorio.
}
