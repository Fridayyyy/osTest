//
// Created by 86184 on 2022/10/26.
//

#include "reader.h"
using namespace std;

void* read(void* arg) {
    auto rw=(RW*)arg;
    semWait(&rw->rMutex);
    rw->readCount++;

    if (rw->readCount==1){
        semWait(&rw->wMutex);
    }

    sem_post(&rw->rMutex);

    cout<<"reader:"<<rw->readCount<<" is reading,n is:"<<rw->num<<endl;
    sleep(2);
    sem_wait(&rw->rMutex);

    rw->readCount--;
    if (rw->readCount==0){
        sem_post(&rw->wMutex);
    }
    sem_post(&rw->rMutex);
    return (void *)NULL;
}