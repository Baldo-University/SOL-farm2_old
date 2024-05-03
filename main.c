#include <stdio.h>
#include <stdlib.h>

#define SOCKETNAME "farm2.sck"


/*
Main del progetto farm2, a.a. 23/24. Autore: Baldini Enrico
Riceve gli argomenti mandati da linea di comando.
*/

/*TODO
Fork

*/


int main(int argc, char *argv[]) {
	
	if(argc<=0) {	//se si invoca il programma senza includere argomenti o se c'e' qualche errore a livello di CL
		fprintf(stderr,"Usare il seguente formato:\n./farm [-n] <int> [-q] <int> [-d] <dir> [-t] <int> file1 [file2,...]\n");	//mettere come define?
		exit(EXIT_FAILURE);
	}
	//TODO fork ed exec di masterworker e collector
	pid_t pid;
	switch(pid=fork()) {
		case -1:	//errore padre
			perror("Fork non riuscita");
			break;
		case 0:	//figlio
			//passare la figlio il nome del socket
			break;
		default:	//padre
			
			break;
	}
}
