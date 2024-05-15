#ifndef MASTERTHREAD

#define MASTERTHREAD
#define WORKERS 4
#define QUEUE_LENGTH 8
#define DELAY 0
#define NAME_LENGTH 256
#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}
#define ec_minusone(s,m) \
	if((s)==-1) {perror(m); exit(EXIT_FAILURE);}
#define ec_negative(s,m) \
	if((s)<0) {perror(m); exit(EXIT_FAILURE);}
	
typedef struct node node;

/*funzionamento di base del thread master

*/
void master_worker(int,char**);

/*aggiunta di un elemento ad una lista

*/
void l_add(node_list*,char*,int);

/*ricerca ricorsiva nella lista di directory passate con "-d"

*/
void dir_search(node_list*,node_list*,node_list*,node_list*);

#endif
