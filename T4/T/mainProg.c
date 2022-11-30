#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include<signal.h>
#include <fcntl.h>
#define hist_size 1024



//获得第一个命令
char *trim(char *string){
    int i = 0;
    int j = 0;
    char *ptr = malloc(sizeof(char*)*strlen(string));
    for (i = 0; string[i] != '\0'; i++)
        if (string[i] != ' ')
        {
            ptr[j] = string[i];
            j++;
        }
    ptr[j] = '\0';
    string = ptr;
    return string;
}

void parse(char *word, char **argv){
    int count = 0;
    memset(argv, 0, sizeof(char*)*(64));
    char *lefts = NULL;
    const char *split = " ";
    //处理字符
    while (1)
    {
        char *p = strtok_r(word, split, &lefts);
        if (p == NULL){
            break;
        }
        argv[count] = p;
        word = lefts;

        count++;
    }
    //退出
    if (strcmp(argv[0], "exit") == 0)
        exit(0);
    else if (strcmp(argv[0], "cd") == 0)
    {
        //切换
        int ch = chdir(argv[1]);
    }
}
//基本命令
void execute(char **argv){
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    {
        printf("fork失败\n");
        exit(1);
    }else if (pid == 0){
        //子进程
        if (execvp(argv[0], argv) < 0 && strcmp(argv[0], "cd"))
            printf("无效命令.\n");
        exit(0);
    }else{
        while (wait(&status) != pid)
            ;
    }
}


void execute_pipe(char **argv, char *output){
    int pfds[2];
    char *file;
    pid_t pid, pid2;
    int status, status2;
    pipe(pfds);
    //pfds[0]:读        pfds[1]:写

    char *args[64];

    int fl = 0;
    if ((pid = fork()) < 0){
        printf("fork失败\n");
        exit(1);
    }if ((pid2 = fork()) < 0){
        printf("fork失败\n");
        exit(1);
    }if (pid == 0 && pid2 != 0){

        close(1);
        dup(pfds[1]);
        close(pfds[0]);
        close(pfds[1]);
        if (execvp(argv[0], argv) < 0)//运行
        {
            close(pfds[0]);
            close(pfds[1]);
            printf("exec错误");
            fl = 1;
            kill(pid2, SIGUSR1);
            exit(0);
        }
    }else if (pid2 == 0 && pid != 0){
        if (fl == 1){
            exit(0);
        }
        parse(output, args);

        close(0);
        dup(pfds[0]);
        close(pfds[1]);
        close(pfds[0]);

        if (execvp(args[0], args) < 0){
            fflush(stdout);
            printf("exec错误 %d", pid);
            kill(pid, SIGUSR1);
            close(pfds[0]);
            close(pfds[1]);
        }
        fflush(stdout);
        printf("HERE");

    }else{
        close(pfds[0]);
        close(pfds[1]);
        while (wait(&status) != pid);
        while (wait(&status2) != pid2);
    }
}




int  main(){
    char *argv[64];
    char *left;
    size_t size = 0;
    int count = 0;
    char *file;
    int i;
    while (1){
        count = 0;
        int flag = 0;
        char *word = NULL;
        char *dire[] = { "pwd" };
        fflush(stdout);
        printf("SHELL~");
        fflush(stdout);
        execute(dire);
        printf("$");

        int len = getline(&word, &size, stdin);
        //空行
        if (*word == '\n')
            continue;

        word[len - 1] = '\0';
        file = NULL;
        i = 0;
        char *temp = (char *)malloc(150);
        strcpy(temp, word);
        parse(temp, argv);


        for (i = 0; word[i] != '\0'; i++){
            if (word[i] == '|'){
                char *p = strtok_r(word, "|", &left);
                flag = 1;
                break;
            }
        }
        if (strcmp(word, "exit") == 0){
            exit(0);
        }
        //管道命令
        if (flag == 1){
            parse(word, argv);
            execute_pipe(argv, left);
        }
        else{
        //其它命令
            parse(word, argv);
            execute(argv);
        }
    }
}