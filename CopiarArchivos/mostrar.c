///////////////////////////////////////////////////////
// Leer el contenido del ficho de entrada y copiarlo en el de salida
//gcc -o ejecutable_mostrar mostrar.c
//./jecutrable_mostrar fichero1 fichero2
///////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



int main ( int argc, char *argv[]){//Numero de rgumeno, Array char de argumntos.

	if (argc =! 3){// nos aseguramos que tenemos el número de arumentos nesarios, en este caso el ficehro de entrada(1) y el de salida(2). La (0) es el ejecuable
		fprintf(stderr, "ERROR EN EL NÚMERO DE ARGUMENTOS");
		exit(-1);		
	}
	int fd1, fd2;//descriptor de fichero
	char buf[150]; // Donde vamos a almacnar loeido del archivo original, tener encuenta que al finl siempre va un "\0"
	int leidos;


// VAMOS A LEER EL FICHERO (1) EN MODO SOLO LECTURA, Si tiene sultado la operacion devolvera un 1; fd =el identificador del archivo.
	if((fd1 = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "ERROR AL ABRIR EL FICHERO DE ENTRADA");
		exit(-2);//CREO QUE INDICA EL NÚMERO DE ERROR
	}

// Abrimos l fichero en modo solo lectura, si no existe se crea, el TRUNC borra loq ue huiese antes.
	if(( fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0 ){
		fprintf(stderr, "ERROR AL ABRIR EL FICHERO DE SALIDA");
		exit(-3);
	}

//Buf no llev apersan por que es un vector, y los vectores y es una direccion de memoria.
//si le pasamos  vector le indicaremos el tamaño del tipo de dato por el numero del tamño del vector.
//leidos, recoge el numero de datos recogidos

//lee fd1, el contenido guadarlo en buf, que es tipo char(tama 1) *150 celdas del vector
	while((leidos = read(fd1, buf, sizeof(char)*150))>0){
		//escribe en fd2, el contenido de buf, untotal de leidos datos
		write(fd2,buf, leidos);
	}
	close(fd1); //Hay que cerrar los archivos abiertos
	close(fd2);	
}
