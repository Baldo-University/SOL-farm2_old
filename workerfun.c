#include <stdio.h>
#include <unistd.h>

#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

/*TODO
Questa e' la funzione utilizzata dai thread del threadpool, il cui risultato e' spedito al processo collector.
Si suppone che i file binari contengono ciascuno soltanto un long per riga
*/

/*
filename e' il nome del file contenente i valori da sommare
socketname e' il nome del file socket a cui si collega il thread
*/

/*da riscrivere?
Inizia col collegamento della socket, si mette in attesa di task.
Una volta ottenuto il task elabora li risultato e lo invia tramite socket.
Terminata l'operazione si rimette in attesa fino all'arrivo del prossimo task
o della propria terminazione
*/

long worker_sum(char *filename) {
	/*calcolo dati nei file*/
	long sum=0;	//somma totale
	long i=0;	//contatore di riga per il calcolo
	FILE *file;	//file da leggere
	ec_null(file=fopen(filename,"r"),"In apertura file");	//apertura file
	char *numbuf;	//buffer che mette un long in store
	//salva un long per riga piu' il newline
	ec_null(numbuf=malloc(sizeof(long)+1),"In malloc di buffer");
	//leggi i valori, un long per riga, e infine inviali tramite socket
	errno=0;	//settato a 0 prima di ogni chiamata per evitare sovrascritture
	while(errno=0, fgets(numbuf,sizeof(long),file)!=NULL) {
		if(errno) {
			perror("worker_sum - in lettura file");
			exit(EXIT_FAILURE);
		}
		sum+=(i++)*(strtol(numbuf,NULL,10));	//i*file[i] seguito da i+1
	}
	errno=0
	fclose(file);
	perror("worker_sum - in chiusura file");
	free(numbuf);
	return sum;
}


void worker_fun(char *socketname) {
	/*Collegamento tramite socket al Collector*/
	long sum;
	char *filename;	//nome di file passato come task
	
	/*Calcolo del risultato*/
	sum=worker_sum(filename);
	
	/*Invio al socket*/
	
	/*Attesa*/
	
	/*Terminazione*/
	
}
