//
// Created by 86184 on 2022/10/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "ComputeTask.h"
#include "IOTask.h"

typedef enum OP{
    COMPUTE_TASK=1,
    IO_TASK
}OP_t;

typedef struct task{
    struct task *next;
    OP_t taskType;
}task_t;

int main(int argc,char *argv[]){
    IOTask io;
    ComputeTask compute;

    double computeTaskTimeElapse=0.0;
    double IOTaskTimeElapse=0.0;
    double totalTimeElapse=0.0;

    struct timeval computeTaskStartTime,computeTaskEndTime,IOTaskStartTime,IOTaskEndTime;

    pid_t fpid;
    task_t computeTask,ioTask;

    task_t *currTask=&computeTask;

    computeTask.taskType=COMPUTE_TASK;
    computeTask.next=&ioTask;
    ioTask.taskType=IO_TASK;
    ioTask.next= nullptr;

    int parentProcess=1;
    int childProcessNum=0;



    while (currTask){
        if(currTask->taskType==IO_TASK)
            gettimeofday(&IOTaskStartTime,NULL);
        else
            gettimeofday(&computeTaskStartTime,NULL);
        fpid=fork();
        if(0==fpid){
            //child process
            parentProcess=0;
            break;
        } else if(-1==fpid){
            printf("Generate child Process error!\n");
            exit(0);
        }
        //wait(NULL);


        printf("Generate child process with pid:%d\n",fpid);

        ++childProcessNum;
        currTask=currTask->next;
    }
    if(parentProcess==0){

        if(currTask->taskType==IO_TASK){
            io.executeIOTask();
            //Print process ID and parent process ID
            printf("This is a IO task, pid:%d parent pid:%d\n",getpid(),getppid());
            gettimeofday(&IOTaskEndTime,NULL);
            IOTaskTimeElapse = (double)(IOTaskEndTime.tv_sec - IOTaskStartTime.tv_sec)*1000.0+(double)(IOTaskEndTime.tv_usec - IOTaskStartTime.tv_usec)/1000.0;
            printf("IO Task Time Consume:%fms\n",IOTaskTimeElapse);

            totalTimeElapse = (double)(IOTaskEndTime.tv_sec-computeTaskStartTime.tv_sec)*1000.0+(double)(IOTaskEndTime.tv_usec-computeTaskStartTime.tv_usec)/1000.0;
            printf("Total Time Consume:%fms \n",totalTimeElapse);

        }
        if(currTask->taskType==COMPUTE_TASK){
            compute.executeComputeTask();
            //Print process ID and parent process ID
            printf("This is a compute task, pid:%d parent pid:%d\n",getpid(),getppid());
            gettimeofday(&computeTaskEndTime,NULL);
            computeTaskTimeElapse = (double)(computeTaskEndTime.tv_sec - computeTaskStartTime.tv_sec)*1000.0+(double)(computeTaskEndTime.tv_usec - computeTaskStartTime.tv_usec)/1000.0;
            printf("Compute Task Time Consume:%fms\n",computeTaskTimeElapse);

        }
        //if(currTask->taskType==COMPUTE_TASK){

    } else{
        wait(NULL);

        wait(NULL);

        //printf("Compute Task Time Consume:%fms, IO Task Time Consume: %fms, Total Time Consume:%fms \n", computeTaskTimeElapse,IOTaskTimeElapse,totalTimeElapse);
    }
}