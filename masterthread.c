#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

/*
MasterThread legge gli argomenti inviati da linea di comando.
Controlla le opzioni e che i file passati siano regolari.
Consideriamo solo una directory aggiuntiva in cui fare una ricerca ricorsiva di file.
*/

/*TODO
Creare la lista di file da passare, se necessario con ricerca ric. nella directory di -d
Gestione di segnali.
Creazione del threadpool

*/

/*costanti*/
#define WORKERS 4	//numero di thread worker di default
#define QUEUE_LENGTH 8	//lunghezza della coda concorrente tra master e worker
#define DELAY 0	//ritardo nell'inserimento di task nella coda
#define NAMELENGTH 255	//lunghezza massima dei filename

/*macro*/
//verifica che un return value sia diverso da -1
#define ec_minusone_passdir(s,m) \
	if((s)==-1) {perror(m);}	

void qualcosa(int argc, char *argv[]) {
	//settiamo i valori di default. Se necessario verranno sovrascritti in seguito dalle opzioni
	long workers=WORKERS;
	long queue_len=QUEUE_LENGTH;
	long delay=DELAY;
	
	int opt;	//integer per getopt
	long nlong;	//long per getopt
	char *extradir;	//nome directory passata con -d
	struct stat info;	//struct per info sulla directory
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
				ec_minusone_passdir(stat(optarg,&info),"In getopt, controllo pathname");
				//verifica che il pathname sia una directory
				if(!S_ISDIR(info.st_mode))
					fprintf(stderr,"Il primo argomento deve essere una directory. Opzione scartata.\n");
				else	extradir=optarg	//se e' directory si segna il nome 
				break;
			default:
				fprintf(stderr,"L'opzione '/%c' passata non e' valida e viene scartata.\n",opt);
				break;
		}
	}
}
