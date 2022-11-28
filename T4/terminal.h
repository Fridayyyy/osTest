//
// Created by 86184 on 2022/11/23.
//

#ifndef OSTEST_TERMINAL_H
#define OSTEST_TERMINAL_H
#include "public.h"

void DisplayTerminal();

int ReadTerminal(char *order,int maxLen);

int AnalysisTerminal(char *order,char **analysisOrder,int maxLen);

int DoTerminal(char **analysisOrder,int sum);

#endif //OSTEST_TERMINAL_H
