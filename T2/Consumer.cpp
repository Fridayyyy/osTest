//
// Created by 86184 on 2022/10/22.
//

#include "Consumer.h"
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "ProducerConsumerUtilities.h"


void getProduct(ProducerConsumerManager_t *producerConsumerManager,int threadIdx){
    sem_t *mutex=&(producerConsumerManager->mutex);
    sem_t *full=&(producerConsumerManager->full);
    sem_t *empty=&(producerConsumerManager->empty);
    int productGetted;

    semWait(full);
    semWait(mutex);

    productGetted=producerConsumerManager->buffer[producerConsumerManager->consumerOffset];
    producerConsumerManager->consumerOffset=(producerConsumerManager->consumerOffset+1)%producerConsumerManager->bufferSize;
    --producerConsumerManager->remainProducts;
    printf("Consumer:%d, get product:%d\n",threadIdx, productGetted);

    semSignal(empty);
    semSignal(mutex);
}

void *consumerThread(void *arg) {
    ThreadStruct_t *curThreadStruct=(ThreadStruct_t *)arg;
    ProducerConsumerManager_t *producerConsumerManager=curThreadStruct->producerConsumerMgr;

    while (curThreadStruct->producerConsumerMgr->exit_flag==0){
        getProduct(producerConsumerManager,curThreadStruct->threadIdx);
        sleep(WAIT_TIME_IN_SECOND);
    }
    return arg;
}

