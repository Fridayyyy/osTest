//
// Created by 86184 on 2022/11/9.
//

#ifndef OSTEST_MAINPROG_H
#define OSTEST_MAINPROG_H
void print_format();
void HeapLocation();
void TextLocation(int (*ptr)(int ,char**));
void DataSegmentLocation();
void file_map();
void StackLocation(int stack_var0, int stack_var1) ;

#endif //OSTEST_MAINPROG_H
