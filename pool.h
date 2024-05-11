#ifndef THREADPOOL

#define THREADPOOL

typedef struct threadpool threadpool;

/*Creazione di un nuovo threadpool*/
threadpool * create_pool(size_t);	

/*termina il threadpool*/
void closepool(threadpool*);

int
#endif
