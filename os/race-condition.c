#include<stdio.h>
#include<pthread.h>

int counter = 0;

void* increment_counter() {
    for(int i=0; i<=4; i++) {
        counter += 1;
    }

    return NULL;
}

int main() {
    pthread_t thread_1, thread_2;

    pthread_create(&thread_1, NULL, (void*)increment_counter, NULL);
    pthread_create(&thread_2, NULL, (void*)increment_counter, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("counter=%d\n", counter);

    return 0;
}