#include <stdio.h>
#include <pthread.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>

int r1 = 1, r2 = 2;
pthread_mutex_t mutex1, mutex2;

void* func1() {
    pthread_mutex_lock(&mutex1);
    r1 = 3;
    printf("Thread 1 changed r1\n");

    sleep(3);
    
    pthread_mutex_lock(&mutex2);
    r2 = 3;
    printf("Thread 1 changed r2\n");

    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
}

void* func2() {
    pthread_mutex_lock(&mutex2);
    r2 = 4;
    printf("Thread 2 changed r2\n");

    sleep(3);
    
    pthread_mutex_lock(&mutex1);
    r1 = 4;
    printf("Thread 2 changed r1\n");

    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}

int main() {
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    
    pthread_t p1, p2;
    pthread_create(&p1, NULL, func1, NULL);
    pthread_create(&p2, NULL, func2, NULL);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    return 0;
}
