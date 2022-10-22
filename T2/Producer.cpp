//
// Created by 86184 on 2022/10/22.
//

#include "Producer.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "ProducerConsumerUtilities.h"
#include "Producer.h"

#define MAX_RAND_NUMBER 9999999
void generateProduct(ProducerConsumerManager_t *producerConsumerManager,int threadIdx){
    sem_t *mutex=&(producerConsumerManager->mutex);
    sem_t *full=&(producerConsumerManager->full);
    sem_t *empty=&(producerConsumerManager->empty);

    int productGenerated=rand()%MAX_RAND_NUMBER;

    semWait(empty);
    semWait(mutex);

    ++producerConsumerManager->remainProducts;
    producerConsumerManager->buffer[producerConsumerManager->producerOffset]=productGenerated;
    producerConsumerManager->producerOffset=(producerConsumerManager->producerOffset+1)%producerConsumerManager->bufferSize;
    printf("producer:%d, generate product:%d\n",threadIdx, productGenerated);

    semSignal(full);
    semSignal(mutex);//after accessing buffer
}
void *producerThread(void *arg){
    ThreadStruct_t *curThreadStruct=(ThreadStruct_t*)arg;
    ProducerConsumerManager_t  *producerConsumerManager=curThreadStruct->producerConsumerMgr;

    srand(curThreadStruct->threadIdx);
    while (curThreadStruct->producerConsumerMgr->exit_flag==0){
        generateProduct(producerConsumerManager,curThreadStruct->threadIdx);
        sleep(WAIT_TIME_IN_SECOND);
    }
    if(curThreadStruct->threadIdx==0){
        while (producerConsumerManager->remainProducts==0){
            do {
                generateProduct(producerConsumerManager,curThreadStruct->threadIdx);
                sleep(1);
            } while (producerConsumerManager->remainProducts!=producerConsumerManager->bufferSize);
        }
    }
    return arg;
}

