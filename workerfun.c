#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define NAME_LENGTH 256
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

/*
Inizia col collegamento della socket, si mette in attesa di task.
Una volta ottenuto il task elabora li risultato e lo invia tramite socket.
Terminata l'operazione si rimette in attesa fino all'arrivo del prossimo task
o della propria terminazione
*/

/*Questa funzione effettua il calcolo di result*/
long worker_sum(char *filename) {
	long sum=0, i=0, temp=0;	//somma totale, indice e valore ausiliario
	size_t nread;	//numero di byte letti
	FILE *file;	//file da leggere
	ec_null(file=fopen(filename,"r"),"worker_sum, s.c. apertura file");	//apertura file con controllo
	char *numbuf;	//buffer che salva l'ultimo long letto
	//allocazione memoria al buffer con controllo
	ec_null(numbuf=malloc(sizeof(long),"worker_sum, s.c. allocazione memoria");	
	while(errno=0, nread=fread(numbuf,sizeof(long),1,file)>0) {	//legge i byte un long alla volta
		if(errno) {
			perror("worker_sum, s.c. lettura file")
			fclose(file);
			perror("worker_sum, s.c. chiusura file")
			free(numbuf);
			return -1;
		}
		temp=strtol(numbuf,NULL,10);	//trasforma il valore da stringa a long
		sum+=i*temp;	//i*file[i]
		i++;
	}
	errno=0;
	fclose(file);
	perror("worker_sum, s.c. chiusura file");
	free(numbuf);
	return sum;
}


void worker_fun(char *socketname) {
	/*Collegamento tramite socket al Collector*/
	long sum;
	char *filename;	//nome di file passato come task
	ec_null(filename=malloc(NAME_LENGTH*sizeof(char)),"worker, s.c. allocazione memoria");
	
	//inizio loop
		/*Consumo di un filename dalla coda di produzione*/
	
		/*Calcolo del risultato*/
		sum=worker_sum(filename);
	
		/*Invio dati tramite socket*/

	//fine loop
	
	/*Terminazione*/	
}
