#ifndef QUEUE
	#define QUEUE
	#define NAME_LENGTH 256
	char **create_queue(size_t);
	int isfull();
	int isempty();
	int enqueue(const char*);
	const char *dequeue();
	void destroy_queue();
#endif
