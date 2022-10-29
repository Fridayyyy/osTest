#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "reader.h"
#include "writer.h"

int main(){

    RW rw;
    pthread_t wid[N_WRITER],rid[N_READER];

    for (int i = 0; i < N_READER; ++i) {
        pthread_create(&wid[i],NULL,write,&rw);
        pthread_create(&rid[i],NULL,read,&rw);
    }

    for (int i = 0; i < N_READER; ++i) {
        pthread_join(rid[i],NULL);
    }
    for (int i = 0; i < N_READER; ++i) {
        pthread_join(wid[i],NULL);
    }

    sleep(10);
    return 0;
}