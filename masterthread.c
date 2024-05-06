#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>

/*
MasterThread legge gli argomenti inviati da linea di comando.
Controlla le opzioni e che i file passati siano regolari.
*/

/*TODO
Creare la lista di file da passare, con eventuale ricerca ric. nella directory di -d
Gestione di segnali.
Funzione atexit()
funzione di ricerca file in maniera ricorsiva nelle directory passate con -d
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
	
//lista di filepath. Valida sia per i filename di file binari sia per i pathanme di directory
//TODO mettere in .h condiviso
typedef struct node {
	char name[NAME_LENGTH+1];
	struct node* next;
} node;
typedef struct node* node_list;

//aggiunta di un filename in testa alla lista di filename (vale per file e per directory)
void l_add(node_list *head, char *name) {
	node_list new;
	ec_null((new=malloc(sizeof(node)))==NULL,"MasterWorker: in malloc di nodo da aggiungere a lista");
	strncpy(new->name,name,NAME_LENGTH+1);
	new->next=*head;
	*head=new;
}

//ricerca ricorsiva nella directory passata di filename e directory da mettere sulla coda di produzione
void dir_produce_r(node_list *dir_head, node_list *dir_aux, node_list *file_head, node_list *file_aux) {
	
}

void master_worker(int argc, char *argv[]) {
	//settiamo i valori di default. Se necessario verranno sovrascritti in seguito dalle opzioni
	long workers=WORKERS;
	long queue_len=QUEUE_LENGTH;
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
				else queue_len=nlong;
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
					l_add(&directories,optarg);	//passa lista e nome directory
				break;
				
			default:
				fprintf(stderr,"L'opzione '/%c' passata non e' valida e viene ignorata.\n",opt);
				break;
		}
	}
	
	//si salvano i filename su di una lista che sara' passata alla coda di produzione	
	node_list files=NULL;	//lista di filename
	node_list file_aux=files;		//puntatore ausiliario
	struct stat file_info;	//info sul file considerato
	//ricerca nomi file passati. Si inizia scorrendo argv[]
	int i;
	for(i=1;i<argc;i++) {
		//prima controlla di non stare considerando un'opzione
		if(!strncmp(argv[i],"-n",3) || !strncmp(argv[i],"-q",3 || !strncmp(argv[i],"-t",3 || !strncmp(argv[i],"-d",3)
			i++;	//se e' un'opzione se ne salta l'argomento
		else {	//controlla sia un file regolare
			ec_minusone(stat(argv[i],"Nella lista di file"));
			if(!S_ISREG(file_info.st_mode))
				fprintf(stderr,"Passato file non adatto.\n");
			else	//aggiungi il file alla lista di filename
				l_add(&files,argv[i]);
		}
	}
}
