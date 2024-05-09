#include <dirent.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pool.h"
#include "queue.h"

/*
MasterThread legge gli argomenti inviati da linea di comando.
Controlla le opzioni e che i file passati siano regolari.
*/

/*TODO
Gestione di segnali.
Funzione atexit()
*/

/*costanti*/
#define WORKERS 4	//numero di thread worker di default
#define QUEUE_LENGTH 8	//lunghezza della coda concorrente tra master e worker
#define DELAY 0	//ritardo nell'inserimento di task nella coda
#define NAME_LENGTH 256	//lunghezza massima dei filename +1

/*macro*/
#define ec_null(s,m) \
	if((s)==NULL) {perror(m); exit(EXIT_FAILURE);}
#define ec_minusone(s,m) \
	if((s)==-1) {perror(m); exit(EXIT_FAILURE);}
#define ec_negative(s,m) \
	if((s)<0) {perror(m); exit(EXIT_FAILURE);}

/*Elementi di una lista di filepath*/
typedef struct {
	char name[NAME_LENGTH];
	struct node* next;
} node;
typedef struct node* node_list;

void l_add(node_list*,char*,int);
void dir_search(node_list*,node_list*,node_list*,node_list*);
int enqueue();

//funzione principale del processo padre
//legge gli argomenti, crea la lista di file da produrre, la coda di produzione e il threadpool
//gestisce i segnali
void master_worker(int argc, char *argv[]) {
	//settiamo i valori di default. Se necessario verranno sovrascritti in seguito dalle opzioni
	long workers=WORKERS;
	size_t queue_len=QUEUE_LENGTH;
	long delay=DELAY;
	
	int opt;	//integer per getopt
	long nlong;	//long per getopt
	node_list directories=NULL;	//lista di directory passate con -d
	Node_list dir_aux=directories;;	//puntatore ausiliario
	struct stat dir_info;	//struct per info su filename passato (directory o file regular)
	//scorre gli argomenti passati
	while((opt=getopt(argc,argv,"n:q:t:d:"))!=-1) {
		switch(opt) {	//guarda se l'argomento e' un'opzione, altrimenti verifica che sia un file
			case 'n':	//numero di worker
				nlong=strtol(optarg,NULL,10);
				if(nlong<1) {
					perror("In getopt");
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero positivo.\n",opt);
				}
				else workers=nlong;
				break;
				
			case 'q':	//lunghezza della coda
				nlong=strtol(optarg,NULL,10);
				if(nlong<1){
					perror("In getopt");
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero positivo.\n",opt);
				}
				else {
					if(nlong>SIZE_MAX) nlong=SIZE_MAX;
					queue_len=(size_t)nlong;
				}
				break;
				
			case 't':	//ritardo di inserimento consecutivo nella coda
				nlong=strtol(optarg,NULL,10);
				if(nlong<0) {
					perror("In getopt");
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero non negativo.\n",opt);
				}
				else delay=nlong;
				break;
				
			case 'd':	//directory di ricerca ricorsiva
				//si ottengono le info sul pathname passato
				ec_minusone(stat(optarg,&dir_info),"In getopt, controllo pathname");
				//verifica che il pathname sia una directory
				if(!S_ISDIR(dir_info.st_mode))
					fprintf(stderr,"L'argomento deve essere una directory. Opzione scartata.\n");
				else		//aggiunge la directory alla lista di directory da esplorare
					l_add(&directories,optarg,0);	//passa lista e nome directory
				break;
				
			default:
				fprintf(stderr,"L'opzione '/%c' passata non e' valida e viene ignorata.\n",opt);
				break;
		}
	}
	
	//creazione della coda di task, un array di stringhe usato come buffer a cerchio
	char **queue=create_queue(queue_len,NAME_LENGTH);
	
	//creazione del threadpool
	
	//si salvano i filename su di una lista che sara' passata alla coda di produzione	
	node_list files=NULL;	//lista di filename
	node_list file_aux=files;		//puntatore ausiliario
	struct stat file_info;	//info sul file considerato
	//ricerca nomi file passati. Si inizia scorrendo argv[]
	int i;
	for(i=optind;i<argc;i++) {	//optarg ha messo le opzioni come primi argomenti, resta solo la lista di file
		ec_minusone(stat(argv[i],"Nella lista di file"));	//ottieni info sul file
		if(!S_ISREG(file_info.st_mode))	//se l'argomento passato non e' un file si ignora
			fprintf(stderr,"Passato file non adatto.\n");
		else l_add(&files,argv[i],0);	//altrimenti aggiungi il file alla lista di filename
	}
	
	//ricerca all'interno della lista di directory
	while(directories!=NULL) {
		dir_search(&directories,&dir_aux,&files,&file_aux);	//ricerca breadth-first
		*directories=directories->next;	//scorre la lista
		free(dir_aux);	//libera la memoria
		dir_aux=*directories;
	}
	
	//ciclo di inserimento task nella coda	
}

/*aggiunta di un filename alla lista di filename (vale per file e per directory)*/
void l_add(node_list *head, char *name, int mode) {	//mode indica se inserire in cima o in coda
	node_list new;
	ec_null((new=malloc(sizeof(node)))==NULL,"MasterWorker: in malloc di nodo da aggiungere a lista");
	strncpy(new->name,name,NAME_LENGTH+1);
	switch(mode) {
		case 0:	//aggiunta in testa
			new->next=*head;
			*head=new;
			break;
		case 1:	//aggiunta in coda
			node_list aux=*head;
			while(aux->next!=NULL)
				aux=aux->head;
			aux->next=*new;
			aux=*head;	//per consistenza rimettiamo il puntatore ausiliario in testa alla lista
			break;
	}
}

/*ricerca ricorsiva nella directory passata di filename e directory da mettere nella lista di filename*/
void dir_search(node_list *dir_head, node_list *dir_aux, node_list *file_head, node_list *file_aux) {
	DIR *dir=opendir(dir_head->name);
	ec_null(dir,"MasterWorker, dir_search, s.c. in apertura");
	struct dirent *file;	//struttura dati per file all'interno della directory che stiamo scorrendo
	while((errno=0, file=readdir)!=NULL) {	
		if(errno!=0) {	//controllo errore
			fprintf(stderr,"%s\t",dir_head->name);
			exit(EXIT_FAILURE);
		}
		if(file->d_type==DT_REG) {	//controlla se e' un file normale
			char fullname[NAME_LENGTH];
			snprintf(fullname,NAME_LENGTH,"%s%s",dir_head->name,file->d_name);
			l_add(&file_head,fullname,0);	//inserimento in testa del file
		}
		else if(file->d_type==DT_DIR && dile->d_name[0]!='.') {	//controlla se e' una directory diversa da "./"
			char fullname[NAME_LENGTH];
			snprintf(fullname,NAME_LENGTH,"%s%s",dir_head->name,file->d_name);	//salva il filepath completo della directory
			l_add(&dir_head,fullname,1);	//inserisci la directory trovata in fondo alla lista di directory
		}
		else if(file->d_type==DT_UNKNOWN) {	//gestione tipo sconosciuto
			perror("MasterWorker, dir_search, tipo file sconosciuto");
			//exit(EXIT_FAILURE);
		}
	}
	if(errno!=0) {	//controllo errore
		fprintf(stderr,"%s\t",dir_head->name);
		exit(EXIT_FAILURE);
	}
}
