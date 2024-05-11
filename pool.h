#ifndef THREADPOOL

#define THREADPOOL

typedef struct threadpool threadpool;

/*Creazione di un nuovo threadpool*/
threadpool * create_pool(size_t);	

/*Aggiunta di un nuovo thread*/
int add_thread(threadpool*);

/*Rimozione di un thread*/
void remove_thread(threadpool*);

/*Terminazione del threadpool*/
void close_pool(threadpool*);

#endif
