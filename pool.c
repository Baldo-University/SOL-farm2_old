#include <pthread.h>
#include <stdio.h>
#include "pool.h"
#include "queue.h"

#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}

/*
Implementazione del threadpool
Composto da una lista di thread, 
*/

//Struct per indicare la lista di thread del threadpool
typedef struct threadnode {
	pthread_t thread;	//thread
	struct threadnode *next;	//puntatore al prossimo thread
} threadnode;
typedef struct threadnode* threadlist;	//definizione aggiuntiva per rendere piu' chiaro il codice

//Struct del threadpool
typedef struct threadpool {
	threadlist threads_head;	//puntatore alla testa di lista thread
	threadlist threads_rear;	//puntatore alla coda di lista thread
	size_t num_threads;		//numero di thread
} threadpool;

extern int threadpool_running=0;	//indica se il threadpool sia attivo o meno
extern pthread_mutex_t poolmutex;	//mutex del threadpool
extern pthread_mutex_t runningmutex;	//mutex di threadpool_running

threadpool * create_pool(size_t size) {
	threadpool *pool;
	ec_null(pool=(threadpool*)malloc(sizeof(threadpool)),"pool, s.c. allocazione memoria al threadpool");
	pool->num_threads=size;
	pthread_mutex_init(&poolmutex,NULL);
	threadpool_running=1;
	return pool;
}

/*Aggiunta di un nuovo thread*/
int add_thread(threadpool*) {

}

/*Rimozione di un thread*/
void remove_thread(threadpool*) {

}

/*Terminazione del threadpool*/
void close_pool(threadpool*) {

}
