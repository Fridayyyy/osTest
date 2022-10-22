//
// Created by 86184 on 2022/10/22.
//

#include "ProducerConsumerUtilities.h"
#include <stdio.h>
#include <stdlib.h>

void initProducerConsumerManager(ProducerConsumerManager_t *producerConsumerManager){
    sem_init(&producerConsumerManager->mutex,0,1);
    sem_init(&producerConsumerManager->full,0,0);
    sem_init(&producerConsumerManager->empty,0,BUFFER_SIZE);

    producerConsumerManager->producerOffset=0;
    producerConsumerManager->consumerOffset=0;
    producerConsumerManager->bufferSize=BUFFER_SIZE;

    producerConsumerManager->exit_flag=0;
}


ProducerConsumerManager_t *createProducerConsumerManager() {
    ProducerConsumerManager_t *producerConsumerManager=(ProducerConsumerManager_t*) malloc(sizeof(ProducerConsumerManager_t));
    if(producerConsumerManager== nullptr){
        printf("Error: unable to create producerConsumerMgr\n");
        return NULL;
    }
    initProducerConsumerManager(producerConsumerManager);
    return producerConsumerManager;
}

void initThreadStruct(ThreadStruct_t *threadStruct,
                                                 ProducerConsumerManager_t *producerConsumerManager, int threadIdx) {
    threadStruct->producerConsumerMgr=producerConsumerManager;
    threadStruct->threadIdx=threadIdx;
}

ThreadStruct_t *createThreadStruct(ProducerConsumerManager_t *producerConsumerManager, int threadIdx) {
    ThreadStruct_t  *threadStruct=(ThreadStruct_t*) malloc(sizeof(ThreadStruct_t));
    if(threadStruct== nullptr){
        printf("Error: unable to create ThreadStruct_t\n");
        return NULL;
    }
    initThreadStruct(threadStruct,producerConsumerManager,threadIdx);
    return threadStruct;
}

void semWait(sem_t *sem) {
    int ret=sem_wait(sem);
    if(ret<0)
    {
        perror("sem_wait fail!");
        exit(EXIT_FAILURE);
    }
}

void semSignal(sem_t *sem) {
    int ret=sem_post(sem);
    if(ret<0)
    {
        perror("sem_post fail!\n");
        exit(EXIT_FAILURE);
    }

}

void semDestroy(sem_t *sem) {
    int ret=sem_destroy(sem);
    if(ret<0)
    {
        perror("sem_destroy run fail\n");
        exit(EXIT_FAILURE);
    }

}