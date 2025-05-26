#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>

#define N 2
#define MAX 1000000

int count = 0;

int turn;
bool interested[N];

void enter_region(int process) {
    int other;
    other = 1 - process;

    interested[process] = true;
    turn = process;

    while(turn==process && interested[other]==true);
}

void leave_region(int process) {
    interested[process] = false;
}

void* increment_count(void *arg) {
    int *process = arg;
    for(int i=0; i<MAX; i++) {
        enter_region(*process);
        count += 1;
        leave_region(*process);
    }

    return NULL;
}

int main() {
    pthread_t thread_1, thread_2;

    int process_ids[2] = {0, 1}; // different memory for process IDs

    pthread_create(&thread_1, NULL, increment_count, &process_ids[0]);
    pthread_create(&thread_2, NULL, increment_count, &process_ids[1]);

    for(int i = 0; i<N; i++) interested[i] = false;

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("%d\n", count);

    return 0;
}