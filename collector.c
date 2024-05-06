#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h> 

#define UNIX_MAX_PATH 108
#define N 256	//lunghezza massima di un filepath +1

#define ec_null(s,m) if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}
#define ec_minusone(s,m) if((s)==-1) {perror(m); exit(EXIT_FAILURE);}
/*
Il collector eviene eseguito tramite exec, quindi avere un main lo rende un secondo programma
*/

/*TODO
Main del collector
struttura dati per mantenere tutti i dati ricevuti dai client
select per capire quale thread scrittore e' pronto a fornire dati
Il server e' single threaded per evitare l'overhead del multithread, gia' introdotto da MW
*/

/*elementi di lista con risultato e filepath*/
typedef struct node {
	long result;	
	char name[N];
	struct node* next;
} node;
typedef struct node* node_list;

/*Stampa la lista*/
void l_print(node_list *head) {
	node_list aux=head;	//puntatore ausiliario
	while(aux!=NULL) {
		fprintf(stdout,"%ld\t%s\n",aux->result,aux->name);
		aux=aux->next;
	}
	fflush(stdout);
}

/*Libera la memoria allocata alla lista*/
void l_clear(node_list *head) {
	node_list aux=head;
	while(head!=NULL) {
		if(head->next!=NULL)
			head=head->next;
		free(aux);
		aux=*head;
	}
}

/*Aggiunge un risultato alla lista in maniera ordinata*/
void l_ordered_add(node_list *head, long res, char *name) {
	node_list new;	//nuovo elemento di lista
	ec_null((new=malloc(sizeof(node)))==NULL,"Collector, in malloc di nodo da aggiungere a lista");
	new->result=res;	//inizializzazione delle componenti del nodo
	strncpy(new->name,name,NAME_LENGTH+1);
	//controlla se l'elemento aggiunto deve andare in testa
	if(head==NULL && head->result>=new->result) {
		new->next=*head;
		*head=new;
	}
	else {	//inserimento all'interno della lista
		node_list aux=head;	//puntatore ausiliario
		while(aux->next!=NULL) {
			//l'elemento a cui punta aux ha valore maggiore dell'el. da inserire
			if(aux->next->result>=new->result) {	//inserimento tra due nodi
				new->next=aux->next;
				aux->next=*new;
				return;
			}
			else *aux=aux->next;	//scorrimento di coda
		}
		//ha scorso la lista e non ha trovato elementi maggiori di quello da includere
		aux->next=*new;
	}
}

/*
Secondo processo del programma, riceve come argomento il nome della socket.
Fa da server per i thread del processo MasterWorker
*/
int main(int argc, char *argv[]) {
	//controlla di aver ricevuto esattamente un argomento (controllo superfluo?)
	if(argc!=2) {
		fprintf(stderr,"Errore fatale, argomenti mal posti in Collector.");
		exit(EXIT_FAILURE);
	}
	node_list results=NULL;	//crea la lista di 
	/*Setup socket*/
	int running=1;	//int che fa da booleano per il while
	int fd_skt, fd_c;	//file descriptor di socket di ascolto passivo e di 
	fd_set set, rdset;	//insieme di fd attivi e ins. di fd attesi in lettura 
	int nread;
	struct sockaddr_un sa;
	strncpy(sa.sun_path,argv[1],UNIX_PATH_MAX);	//copia nel sockaddr il pathname del socket
	sa.sun_family=AF_UNIX;
	fd_skt=socket(AF_UNIX,SOCK_STREAM,0);	//creazione socket
	ec_minusone(fd_skt,"Collector, creazione socket");
	//dare nome alla socket
	ec_minusone(bind(fd_skt,(struct sockaddr *)&sa,sizeof(sa)),"Collector, bind");
	ec_minusone(listen(fd_skt,SOMAXCONN),"Collector, listen");	//segna socket in ascolto
	FD_ZERO(&set);
	FD_SET(fd_skt,&set);
	while(running) {
		rdset=set;
		
	}
}
