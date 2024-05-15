#ifndef THREADPOOL

#define THREADPOOL

typedef struct threadpool threadpool;
typedef struct threadnode threadnode;

/*Creazione di un nuovo threadpool
argomenti passati: numero iniziale di thread, coda di produzione
returns: threadpool
*/
threadpool * create_pool(size_t,char**);	

/*Aggiunta di un nuovo thread
argomenti passati:
returns:
*/
int add_thread(threadpool*);

/*Rimozione di un thread

*/
void remove_thread(threadpool*);

/*Terminazione del threadpool

*/
void close_pool(threadpool*);

#endif
