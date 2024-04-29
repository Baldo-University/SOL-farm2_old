#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
Main del progetto farm2, a.a. 23/24. Autore: Baldini Enrico
Riceve gli argomenti mandati da linea di comando
*/


int main(int argc, char *argv[]) {
	
	if(argc<=0) {	//se si invoca il programma senza includere argomenti o se c'e' qualche errore a livello di CL
		fprintf(stderr,"Usare il seguente formato:\n./farm [-n] <int> [-q] <int> [-d] <dir> [-t] <int> file1 [file2,...]\n");
		exit(EXIT_FAILURE);
	}
	
	
	
	
}
