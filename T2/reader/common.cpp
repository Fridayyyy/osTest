//
// Created by 86184 on 2022/10/29.
//
#include "common.h"

RW::RW() {
    sem_init(&rMutex,0,1);
    sem_init(&wMutex,0,1);
    readCount=0;
    i=0;
    num=-1;
}
RW::~RW() {
    semDestroy(&rMutex);
    semDestroy(&wMutex);
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