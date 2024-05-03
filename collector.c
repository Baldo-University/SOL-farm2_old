#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h> 

#define UNIX_MAX_PATH 108
#define N 255	//lunghezza massima di un filepath 

#define ec_minusone(s,m) \
	if((s)==-1) {perror(m); exit(EXIT_FAILURE);}
/*
Il collector eviene eseguito tramite exec, quindi avere un main lo rende un secondo programma
*/

/*TODO
Main del collector
struttura dati per mantenere tutti i dati ricevuti dai client
select per capire quale thread scrittore e' pronto a fornire dati
Il server e' single threaded per evitare l'overhead del multithread, gia' introdotto da MW
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
	int running=1;	//int che fa da booleano per il while
	int fd_skt, fd_c;	//file descriptor di socket di ascolto passivo e di 
	fd_set set, rdset;	//insieme di fd attivi e ins. di fd attesi in lettura 
	int nread;
	struct sockaddr_un sa;
	strncpy(sa.sun_path,argv[1],UNIX_PATH_MAX);	//copia nel sockaddr il pathname del socket
	sa.sun_family=AF_UNIX;
	fd_skt=socket(AF_UNIX,SOCK_STREAM,0);	//creazione socket
	ec_minusone(fd_skt,"Collector, creazione socket");
	//dare nome alla socket
	ec_minusone(bind(fd_skt,(struct sockaddr *)&sa,sizeof(sa)),"Collector, bind");
	ec_minusone(listen(fd_skt,SOMAXCONN),"Collector, listen");	//segna socket in ascolto
	FD_ZERO(&set);
	FD_SET(fd_skt,&set);
	while(running) {
		rdset=set;
		
	}
}
