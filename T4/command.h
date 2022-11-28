//
// Created by 86184 on 2022/11/23.
//

#ifndef OSTEST_COMMAND_H
#define OSTEST_COMMAND_H
#include "public.h"

void ls();
void cd(char **analysisOrder,int sum);
void touch(char **analysisOrder,int sum);
void cp(char *src,char *dst);
void help();
void mkdir();

#endif //OSTEST_COMMAND_H
