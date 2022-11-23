#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;



void maps(char s[8]){
    char str[100] = {"cat /proc/"};
    strcat(str,s);
    char l[8] = {"/maps"};
    strcat(str,l);
    cout<<str<<endl;
    system(str);
}


void status(char s[8]){
    char str[100] = {"cat /proc/"};
    strcat(str,s);
    char l[8] = {"/status"};
    strcat(str,l);
    cout<<str<<endl;
    system(str);
}

int main(){
    char *p[8];
    char s[8];
    int pid = getpid();
    sprintf(s,"%d",pid);
    cout<<"分配内存前的maps和status情况"<<endl;
    maps(s);
    status(s);
    cout<<"先分配6个128MB的内存 然后删除235号"<<endl;

    p[0] = (char *)malloc(128*1024*1024);
    cout<<"分配一个后的情况"<<endl;
    maps(s);
    status(s);
    p[1] = (char *)malloc(128*1024*1024);
    p[2] = (char *)malloc(128*1024*1024);
    p[3] = (char *)malloc(128*1024*1024);
    p[4] = (char *)malloc(128*1024*1024);
    p[5] = (char *)malloc(128*1024*1024);
    cout<<"分配完的情况"<<endl;
    maps(s);
    status(s);
    delete[] p[1];
    delete[] p[2];
    delete[] p[4];
    cout<<"删除的情况"<<endl;
    maps(s);
    status(s);

    cout<<"分配一个1024MB内存..."<<endl;
    p[6] = (char *)malloc(1024*1024*1024);
    cout<<"分配1024MB的情况"<<endl;
    maps(s);
    status(s);

    cout<<"分配一个64MB的内存..."<<endl;
    p[7] = (char *)malloc(64*1024*1024);
    cout<<"分配64MB的情况"<<endl;
    maps(s);
    status(s);
    delete[] p[0];
    delete[] p[3];
    delete[] p[5];
    delete[] p[6];
    delete[] p[7];
    return 0;
}