#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIX_MAX_PATH 108

#define ec_minusone(s,m) \
	if((s)==-1) {perror(m); exit(EXIT_FAILURE);}
/*
Il collector eviene eseguito tramite exec, quindi avere un main lo rende un secondo programma
*/

/*TODO
Main del collector
*/

/*
Secondo processo del programma, riceve come argomento il nome della socket.
Fa da server per i thread del processo MasterWorker
*/
int main(int argc, char *argv[]) {
	//controlla di aver ricevuto esattamente un argomento (controllo superfluo?)
	if(argc!=2) {
		fprintf(stderr,"Errore fatale: argomenti mal posti in Collector.");
		exit(EXIT_FAILURE);
	}
	/*Setup socket*/
	int fd_skt, fd_c;
	struct sockaddr_un sa;
	strncpy(sa.sun_path,argv[1],UNIX_PATH_MAX);
	fd_skt=socket(AF_UNIX,SOCK_STREAM,0);	//creazione socket
	ec_minusone(fd_skt,"Collector, creazione socket");
	
}
