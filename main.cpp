#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_WRITER 10
#define N_READER 10

using namespace std;
sem_t rMutex,wMutex;
int readCount=0;

struct argument{
    int i;
    int n;
};

void * writer(void * arg){
    sem_wait(&wMutex);
    ((argument*)arg)->n+=rand()%20;
    cout<<"writer:"<<((argument*)arg)->i<<" is writing,num is:"<<((argument*)arg)->n<<endl;
    sleep(1);

    sem_post(&wMutex);
}

void * reader(void * arg){
    sem_wait(&rMutex);
    readCount++;

    if (readCount==1){
        sem_wait(&wMutex);
    }

    sem_post(&rMutex);

    cout<<"reader:"<<readCount<<" is reading,n is:"<<((argument*)arg)->n<<endl;
    sleep(1);
    sem_wait(&rMutex);

    readCount--;
    if (readCount==0){
        sem_post(&wMutex);
    }
    sem_post(&rMutex);
}
int main(){
    argument arg;
    arg.n=-1;

    sem_init(&rMutex,0,1);
    sem_init(&wMutex,0,1);

    pthread_t wid[N_WRITER],rid[N_READER];

    for (int i = 0; i < N_WRITER; ++i) {
        arg.i=i;
        pthread_create(&rid[i],NULL,reader,&arg);
        pthread_create(&wid[i],NULL,writer,&arg);
        sleep(1);
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