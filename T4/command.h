//
// Created by 86184 on 2022/11/23.
//

#ifndef OSTEST_COMMAND_H
#define OSTEST_COMMAND_H
#include "public.h"

char *trim(char *string);
void parse(char *word, char **argv);
void execute(char **argv);
void  execute_file(char **argv, char *output);

void  execute_input(char **argv, char *output);
void execute_pipe(char **argv, char *output);
void execute_pipe2(char **argv, char **args, char **argp);

#endif //OSTEST_COMMAND_H
