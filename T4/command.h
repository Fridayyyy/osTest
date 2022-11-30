//
// Created by 86184 on 2022/11/23.
//

#ifndef OSTEST_COMMAND_H
#define OSTEST_COMMAND_H
#include "public.h"

char *trim(char *string);
void parse(char *word, char **argv);
void execute(char **argv);
void execute_pipe(char **argv, char *output);


#endif //OSTEST_COMMAND_H
