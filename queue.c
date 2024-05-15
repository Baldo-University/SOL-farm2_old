#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

char **queue;	//coda
int front=-1, rear=-1;	//indici di inizio e fine 
size_t dim=1;	//dimensione della coda
char *filename;		//buffer per contenere un filename temporaneo
extern pthread_mutex_t queue_lock;	//lock sulla coda
pthread_cond_t cond;		//variabile di condizione sulla coda
extern int queue_running=0;	//variabile condivisa. Indica se MasterWorker continua ad inserire task in coda

//crea la coda
char **create_queue(size_t size) {
	dim=size;
	ec_null(queue=malloc(dim*sizeof(char*)),"queue, s.c. creazione coda");
	pthread_mutex_init(&lock,NULL);
	int i;
	for(i=0;i<dim;i++)	//inizializzazione degli elementi della coda
		ec_null(queue[i]=malloc(NAME_LENGTH*sizeof(char)),"queue, s.c. creazione elemento di coda");
	ec_null(filename=malloc(NAME_LENGTH*sizeof(char)),"queue, s.c. creazione elemento temporaneo");
	queue_running=1;
	return queue;
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
	if(isfull()) 	//controllo di coda piena
		return 1;	//restituisce un codice di errore
	if(!queue_running)	//controllo che la coda non sia stata bloccata
		return 1;
	if(front==-1)	//controlla se la coda e' vuota
		front=0;	//setta l'inizio della coda all'indice 0
	rear=(++rear)%dim;	//la coda si "allunga"
	strncpy(queue[rear],filename,NAME_LENGTH);
	return 0;
}

//Rimozione di un elemento dalla cima della coda
//Return value: char* (stringa) con filename se la funzione termina con successo, NULL se la coda e' vuota
const char *dequeue() {
	if(isempty())
		return NULL;
	strncpy(filename,queue[front],NAME_LENGTH);
	if(front==rear) {	//controlla se con questa chiamata la coda si svuota
		front=-1;
		rear=-1;
	}
	else	//L'inizio della coda scorre al prossimo indice di array
		front=(++front)%dim;
	return filename;
}

//Libera la memoria allocata per la coda e imposta le variabil per indicare che la coda e' chiusa
void destroy_queue() {
	if(queue_running)
		queue_running=0;
	int i;
	for(i=0;i<dim;i++)
		free(queue[i]);
	free(queue);
	free(filename);
	front=-1;
	rear=-1;
	dim=-1;
}
