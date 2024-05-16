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
typedef struct threadpool_t {
	threadlist threads_head;	//puntatore alla testa di lista thread
	threadlist threads_rear;	//puntatore alla coda di lista thread
	size_t num_threads;		//numero di thread
	pthread_mutex_t lock;	//mutex del threadpool
	pthread_cond_t cond;	//condizionale per wait e signal
	char **task_queue;		//coda di task
	int running;		//booleano di pool attivo
} threadpool_t;


threadpool_t * create_pool(size_t size, char **queue) {
	threadpool *pool;
	ec_null(pool=(threadpool*)malloc(sizeof(threadpool)),"pool, s.c. allocazione memoria al threadpool");
	pool->num_threads=size;
	pthread_mutex_init(&poolmutex,NULL);
	threadpool_running=1;
	return pool;
}

/*Aggiunta di un nuovo thread*/
int add_thread(threadpool_t *pool) {

}

/*Rimozione di un thread*/
void remove_thread(threadpool_t *pool) {

}

/*Terminazione del threadpool*/
void close_pool(threadpool_t *pool) {

}
