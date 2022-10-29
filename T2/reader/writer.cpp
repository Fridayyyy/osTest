//
// Created by 86184 on 2022/10/26.
//

#include "writer.h"
#include "common.h"

void *write(void * arg) {
    auto rw=(RW*)arg;
    semWait(&rw->wMutex);
    rw->num+=rand()%20;
    std::cout<<"writer:"<<rw->i<<" is writing,num is:"<<rw->num<<std::endl;
    sleep(1);
    semSignal(&rw->wMutex);
    return (void *)NULL;
}