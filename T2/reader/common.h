//
// Created by 86184 on 2022/10/28.
//


#ifndef OSTEST_COMMON_H
#define OSTEST_COMMON_H

#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

const int N_WRITER=10;
const int N_READER=10;

class RW{
public:
    sem_t rMutex;
    sem_t wMutex;
    int readCount;
    int i;//第i个
    int num;
    RW();
    ~RW();
};
void semWait(sem_t *sem);
void semSignal(sem_t *sem);
void semDestroy(sem_t *sem);


#endif //OSTEST_COMMON_H
