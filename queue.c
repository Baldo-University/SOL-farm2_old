#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

char **items;	//coda
int front=-1, rear=-1;	//indici di inizio e fine 
size_t dim=1;	//dimensione della coda
pthread_mutex_t items_lock;	
pthread_cond_t cond;
extern int isgoing=0;	//variabile condivisa. Indica se MasterWorker continua ad inserire task in coda

//crea la coda
char **create_queue(size_t size) {
	dim=size;
	items=malloc(dim);
	ec_null(items=malloc(dim*sizeof(char*)),"queue, s.c. creazione coda");
	pthread_mutex_init(&lock,NULL);
	int i;
	for(i=0;i<dim;i++)	//inizializzazione degli elementi della coda
		ec_null(items[i]=malloc((NAME_LENGTH)*sizeof(char)),"queue, s.c. creazione elemento di coda");
	return items;
}

//Controlla se la coda e' piena
int isfull() {
	if((front==rear+1) || (front==0 && rear==dim-1)) return 1;	//coda piena
	return 0;	//coda non piena
}

//Controlla se la coda e' vuota
int isempty() {
	if(front==-1) return 1;	//coda vuota
	return 0;	//coda non vuota
}
//Aggiunta di un elemento in fondo alla coda
//return value: 0 se la funzione termina con successo, 1 se la coda e' piena
int enqueue(const char *filename) {
	//controllo di coda piena
	if(isfull()) 
		return 1;	//restituisce un codice di errore
	if(front==-1)	//controlla se la coda e' vuota
		front=0;	//setta l'inizio della coda all'indice 0
	rear=(++rear)%dim;	//la coda si "allunga"
	strncpy(items[rear],filename,NAME_LENGTH);
	return 0;
}

//Rimozione di un elemento dalla cima della coda
//Return value: char* (stringa) con filename se la funzione termina con successo, NULL se la coda e' vuota
const char *dequeue() {
	if(isempty())
		return NULL;
	char *filename;	//stringa temporanea
	ec_null(filename=malloc(NAME_LENGTH*sizeof(char)),"queue, dequeue, s.c. allocazione memoria");
	strncpy(filename,items[front],NAME_LENGTH);
	if(front==rear) {	//controlla se con questa chiamata la coda si svuota
		front=-1;
		rear=-1;
	}
	else	//L'inizio della coda scorre al prossimo indice di array
		front=(++front)%dim;
	return filename;
}

void destroy_queue() {
	
}
