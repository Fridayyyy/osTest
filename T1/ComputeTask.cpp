//
// Created by 86184 on 2022/10/21.
//

#include "ComputeTask.h"
#include <stdio.h>
#include <stdint.h>
#define LOOP_SIZE (1024*1024*1024)

void ComputeTask::executeComputeTask() {
    size_t idx=0;
    size_t loopSize=LOOP_SIZE;

    for (idx = 0; idx< loopSize; ++idx) {
        //sum
    }
}
