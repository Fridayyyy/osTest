//
// Created by 86184 on 2022/10/22.
//

#ifndef OSTEST_PRODUCERCONSUMERUTILITIES_H
#define OSTEST_PRODUCERCONSUMERUTILITIES_H
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#define BUFFER_SIZE 10
#define WAIT_TIME_IN_SECOND 1

typedef struct {
    sem_t mutex;
    sem_t full;
    sem_t empty;

    int producerOffset;
    int consumerOffset;

    int remainProducts;
    int buffer[BUFFER_SIZE];
    int bufferSize;

    int exit_flag;
}ProducerConsumerManager_t;

typedef struct {
    ProducerConsumerManager_t *producerConsumerMgr;
    pthread_t tid;
    int threadIdx;
}ThreadStruct_t;

ProducerConsumerManager_t *createProducerConsumerManager();

void initThreadStruct(ThreadStruct_t *threadStruct,ProducerConsumerManager_t *producerConsumerManager,int threadIdx);

ThreadStruct_t *createThreadStruct(ProducerConsumerManager_t *producerConsumerManager,int threadIdx);
void semWait(sem_t *sem);
void semSignal(sem_t *sem);
void semDestroy(sem_t *sem);


#endif //OSTEST_PRODUCERCONSUMERUTILITIES_H
