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
	void master_worker(int,char**);
	void l_add(node_list*,char*,int);
	void dir_search(node_list*,node_list*,node_list*,node_list*);
#endif
