#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SOCKETNAME "farm2.sck"
#define ec_minusone(s,m) \
	if((s)!=NULL && (s)==-1) {perror(m); exit(EXIT_FAILURE)}

/*
Main del progetto farm2, a.a. 2023/24. Autore: Baldini Enrico
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
	switch(fork()) {
		case -1:	//fork non riuscita
			perror("Main, fork non riuscita");
			exit(EXIT_FAILURE);
			break;
		case 0:	//figlio. Nota: il nome del programma chiamato con exec e' collector
			execl("./collector","collector",SOCKETNAME,(*char)NULL);
			perror("Main, in exec");
			exit(EXIT_FAILURE);
		default:	//padre
			
			break;
	}
	return 0;
}
