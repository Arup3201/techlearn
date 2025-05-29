#define N 100
#define count 0

#define FALSE 0
#define TRUE 1

void producer(void) {
	while(TRUE) {
		item = produce_item();

		if(count==N) sleep();

		insert_item(item);
		count = count + 1;

		if(count==1) wakeup(consumer);
	}
}

void consumer(void) {
	while(TRUE) {
		if(count==0) sleep();
		
		item = remove_item();
		count = count - 1;

		if(count==N-1) wakeup(producer);
	}
}
