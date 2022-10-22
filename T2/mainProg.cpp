#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ProducerConsumerUtilities.h"
#include "Consumer.h"
#include "Producer.h"

#define PRODUCER_NUMBER 2
#define CONSUMER_NUMBER 10

int main(int argc,char *argv[]){
    int idx=0;
    ThreadStruct_t *producerThreadStructPtr=(ThreadStruct_t*) malloc(sizeof(ThreadStruct_t)*PRODUCER_NUMBER);
    ThreadStruct_t *consumerThreadStructPtr=(ThreadStruct_t*) malloc(sizeof(ThreadStruct_t)*CONSUMER_NUMBER);

    ProducerConsumerManager_t *producerConsumerManager=createProducerConsumerManager();
    if(producerThreadStructPtr== nullptr||consumerThreadStructPtr== nullptr||producerConsumerManager== nullptr){
        perror("malloc fail!\n");
        exit(EXIT_FAILURE);
    }
    for (int idx = 0; idx < PRODUCER_NUMBER; ++idx) {
        ThreadStruct_t *curProducerThreadStruct=producerThreadStructPtr+idx;
        initThreadStruct(curProducerThreadStruct, producerConsumerManager, idx);
        pthread_create(&curProducerThreadStruct->tid, NULL, producerThread, (void *)curProducerThreadStruct);

    }
    for(idx = 0; idx < CONSUMER_NUMBER; ++idx){
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        initThreadStruct(curConsumerThreadStruct, producerConsumerManager, idx);
        pthread_create(&curConsumerThreadStruct->tid, NULL, consumerThread, (void *)curConsumerThreadStruct);
    }

    do{
        char aChar = getchar();
        if (aChar == 'q')
        {
            producerConsumerManager->exit_flag = 1;
        }
    }while(producerConsumerManager->exit_flag == 0);

    for(idx = 0; idx < PRODUCER_NUMBER; ++idx){
        ThreadStruct_t *curProducerThreadStruct = producerThreadStructPtr + idx;
        pthread_join(curProducerThreadStruct->tid, NULL);
    }

    //Wait consumer threads exit
    for(idx = 0; idx < CONSUMER_NUMBER; ++idx){
        ThreadStruct_t *curConsumerThreadStruct = consumerThreadStructPtr + idx;
        pthread_join(curConsumerThreadStruct->tid, NULL);
    }

    free(producerThreadStructPtr);
    free(consumerThreadStructPtr);
    semDestroy(&(producerConsumerManager->mutex));
    semDestroy(&(producerConsumerManager->full));
    semDestroy(&(producerConsumerManager->empty));
    free(producerConsumerManager);
    return 1;
}