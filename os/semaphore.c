#include<stdio.h>
#define TRUE 1
#define FALSE 0
#define N 10 // buffer size

typedef int semaphore;

semaphore full=0; // counts full buffer slots
semaphore empty=N; // counts empty buffer slots
semaphore mutex=1; // controls access to critical region

void producer() {
	int item;

	while(TRUE) {
		item=produce_item();
		down(&empty);
		down(&mutex);
		insert_item(item); // critical section
		up(&mutex);
		up(&full);
	}
}

void consumer() {
	int item;

	while(TRUE) {
		down(&full);
		down(&mutex);
		item=remove_item();
		up(&mutex);
		up(&empty);
		consume_item(item);
	}
}

int main() {
	
	return 0;
}
