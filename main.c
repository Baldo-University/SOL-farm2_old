#include <stdio.h>
#include <stdlib.h>

/*
Main del progetto farm2, a.a. 23/24. Autore: Baldini Enrico
Riceve gli argomenti mandati da linea di comando.
*/

/*TODO
La fork e' da fare qui o nel masterWorker? "Il pr. Collector viene generato dal pr. MW"
*/


int main(int argc, char *argv[]) {
	
	if(argc<=0) {	//se si invoca il programma senza includere argomenti o se c'e' qualche errore a livello di CL
		fprintf(stderr,"Usare il seguente formato:\n./farm [-n] <int> [-q] <int> [-d] <dir> [-t] <int> file1 [file2,...]\n");
		exit(EXIT_FAILURE);
	}
	//TODO fork ed exec di masterworker e collector
	pid_t pid;
	switch(pid=fork()) {
		case -1:	//errore padre
			perror("Fork non riuscita");
			break;
		case 0:	//figlio
		
			break;
		default:	//padre
			break;
	}
}
