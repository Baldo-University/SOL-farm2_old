#include <stdio.h>
#include <time.h>

#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

/*TODO
FUNZIONE DI BASE
*/
/*
Questa e' la funzione utilizzata dai thread del threadpool, il cui risultato e' spedito al processo collector.
La funzione prende un filename, verificato essere un file regolare, e applica il calcolo richiesto dal progetto.
*/
#define TIME 1	//intervalli di 1 secondo di tempo

long sum(char *filename, long *result) {
	time_t time;	//"orologio" di ritorno dati.
	FILE *file;	//file da leggere
	ec_null(file=fopen(filename,"r"),"In apertura file");	//apertura file
	char *numstring;	//buffer che mette un long in store
	ec_null(numstring=malloc(sizeof(long)+1),"In malloc di buffer");
	while() {	//leggi i valori, un long per riga, e ogni secondo restituiscili al thread.
		
	}
}
