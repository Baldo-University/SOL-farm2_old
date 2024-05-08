#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 256
#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

char **items;	//coda
int front, rear;	//indici di inizio e fine 
size_t size;	//dimensione della coda

//crea la coda
char **create_queue(size_t size) {
	dim=size;
	items=malloc()
	ec_null(items=malloc(size*sizeof(char*)),"queue, s.c. creazione coda");
	int i;
	for(i=0;i<size;i++)	//inizializzazione degli elementi della coda
		ec_null(items[i]=malloc((NAME_LENGTH)*sizeof(char)),"queue, s.c. creazione elemento di coda");
	return items;
}

//Controlla se la coda e' piena
int isfull() {
	if((front==rear+1) || (front==0 && rear==size-1)) return 1;	//coda piena
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
	rear=(++rear)%size;	//la coda si "allunga"
	strncpy(items[rear],filename,NAME_LENGTH);
	return 0;
}

//Rimozione di un elemento dalla cima della coda
//Return value: char* (stringa) con filename se la funzione termina con successo, NULL se la coda e' vuota
char *dequeue() {
	if(isempty())
		return NULL;
	char filename[NAME_LENGTH];	//stringa temporanea
	strncpy(temp,items[front],NAME_LENGTH);
	if(front==rear) {	//controlla se con questa chiamata la coda si svuota
		front=-1;
		rear=-1;
	}
	else	//L'inizio della coda scorre al prossimo indice di array
		front=(++front)%size;
	return filename;
}
