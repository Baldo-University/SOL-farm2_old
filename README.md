Main
-prende gli argomenti da linea di comando
-Fork ed exec per avere due processi. Il padre è masterworker, il figlio è collector
-Il padre lancia masterworker, con argc e argv come argomenti
-Il figlio chiama execv prendendo il nome della socket come argomento

MasterWorker
-getopt, salvandosi le opzioni in apposite variabili e le directory in una lista di filepath DirList
-Scorre gli argomenti restanti usando optind come indice, controlla se gli argomenti passati da linea di comando sono file  e se lo sono ne inserisce i filepath in una lista di filepath FileList
-Scorre DirList. Per ogni elem di lista:
	-Si assicura di lavorare su una directory
	-Apre la directory
	-chiama readdir() e salva la struct di dirent
	-scorre i file all’interno, vedendo se sono file normali (binari) o directory
	-I primi li mette nella Filelist, i secondi nella DirList
-Crea la coda di task, una struttura dati di dimensione fissa, passata con -q o 8 di default, dotata di mutex.
-Lancia il threadpool, passando il numero di thread che deve avere inizialmente
-Gestisce i segnali SIGINT, SIGUSR1, SIGUSR2, ecc.
-Produce i task da inserire in coda, entrando in un loop
-Terminato il loop, o se interrotto prima, MasterWorker:
	-Avverte il Threadpool di uccidere i thread lanciati e di chiudersi
	-Attende che collector termini
	-chiude

Threadpool
-Crea una lista di thread di dimensione variabile
-Aumenta o diminuisce il numero di thread in base alle richieste di MasterWorker
-Spenge i thread quando il masterworker lo ordina
-Avverte collector che i thread sono tutti cessati e può smettere di ascoltare

WorkerFunction
-Si collega al socket di Collector
-Entra in un loop nel quale:
	-Si mette in attesa di task da consumare
	-Ottenuto un task svolge i calcoli richiesti
	-invia risultato e filepath tramite socket
	-esce dal loop se Threadpool lo ordina
-Si scollega dal socket

Collector
-Crea una lista su cui salvare via via i risultati ottenuti in maniera ordinata
-Mette a disposizione una socket di ascolto
-Utilizza select() per capire quando un socket ha qualcosa da comunicare
-Mette su un thread che dorme per un secondo e al termine di quel secondo chiede la stampa ordinata dei risultati parziali
-Salva i risultati ottenuti dalle socket all’interno della struttura dati.
-Una volta chiusi tutti i thread, chiude la socket. (Lo vede da un segnale ricevuto da MW)
-Stampa ordinata dei risultati nella struttura dati.
-Return del proprio esito al processo MasterWorker
