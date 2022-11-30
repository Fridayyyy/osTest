//
// Created by 86184 on 2022/11/23.
//
#include "public.h"
#include "command.h"



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