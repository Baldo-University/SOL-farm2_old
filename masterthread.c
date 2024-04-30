#include <stdio.h>
#include <stdlib.h>

/*costanti*/
#define WORKERS 4	//numero di thread worker di default
#define QUEUE_LENGTH 8	//lunghezza della coda concorrente tra master e worker
#define DELAY 0	//ritardo nell'inserimento di task nella coda
#define NAMELENGTH 255	//lunghezza massima dei filename

/*macro*/
//verifica che un return value sia diverso da -1
#define ec_minusone_passdir(s,m) \
	if((s)==-1) {perror(m); fprintf(stderr,"Opzione '-d' passata non e' una directory. Viene ignorata.\n");}

void qualcosa(int argc, char *argv[]) {
	//settiamo i valori di default. Se necessario verranno sovrascritti in seguito dalle opzioni
	int workers=WORKERS;
	int queue_len=QUEUE_LENGTH;
	long delay=DELAY;
	
	int opt, nint;	//integer per getopt
	char *extradir;
	//scorre gli argomenti passati
	while((opt=getopt(argc,argv,"n:q:t:d:"))!=-1) {
		switch(opt) {	//guarda se l'argomento e' un'opzione, altrimenti verifica che sia un file
			case 'n':	//numero di worker
				nint=strtol(optarg,NULL,10);
				if(nint<1)
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero positivo.\n",opt);
				else workers=(int)nint;
				break;
			case 'q':	//lunghezza della coda
				nint=strtol(optarg,NULL,10);
				if(nint<1)
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero positivo.\n",opt);
				else queue_len=(int)nint;
				break;
			case 't':	//ritardo di inserimento consecutivo nella coda
				nint=strtol(optarg,NULL,10);
				if(nint<0)
					fprintf(stderr,"L'opzione '-%c' passata non e' valida e viene scartata. Passare un intero non negativo.\n",opt);
				else delay=nint;
				break;
			case 'd':	//directory di ricerca ricorsiva
				//prova
				break;
			default:
				fprintf(stderr,"L'opzione '/%c' passata non e' valida e viene scartata.\n",opt);
				break;
		}
	}
}
