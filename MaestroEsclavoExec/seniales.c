#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

/*
Señales, son eventos que ocurren externos al programa
Se pueden interactuar con  la mayoria de señales.
Con signal action(int,struct*,struct), cambiaremos las opciones de las señales.
Todo lo que hagamos de señales se hara al principio del programa, por que no sabemos si llegara, ni cuando llegara.
*/

void funcion_manejador(int n){
	printf("CARITA SAOOOO :)\n");
}
int main(){
	struct sigaction sa;
	sa.sa_handler = funcion_manejador; //punetero a funcion
	if(sigaction(SIGINT,&sa,NULL)<0){
		perror("-----------");
		exit(-1);
	}

	while(1){
		sleep(2);
    	printf("Falta algo %d\n",getpid());
	}
}
//si da problema.sa_flags00;

//man 7 signal veremos el valor numerico de las ordenes
// por ejemplo con kill -9 numeroproceso podremos matar el proceso desde otra terminal



//Veremos que no acaba y que al hacer contro c sale la carita
//para salir: control+z y poner kill %%
