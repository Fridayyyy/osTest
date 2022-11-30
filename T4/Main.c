//
// Created by 86184 on 2022/11/23.
//
#include "public.h"
#include "command.h"

char *hist[hist_size];
int f = 0;
int head = 0, filled = 0;


int  main()
{
    char line[1024];
    char *argv[64];
    char *args[64];
    char *left;
    size_t size = 0;
    char ch;
    int count = 0;
    char *tri;
    char *second;
    char *file;
    int i;
    for (i = 0; i < hist_size; i++)
    {
        hist[i] = (char *)malloc(150);
    }

    while (1)
    {
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
        if (*word == '\n')
            continue;

        word[len - 1] = '\0';
        char *file = NULL;
        int i = 0;
        char *temp = (char *)malloc(150);
        strcpy(temp, word);
        parse(temp, argv);

        strcpy(hist[(head + 1) % hist_size], word);
        head = (head + 1) % hist_size;
        filled = filled + 1;

        for (i = 0; word[i] != '\0'; i++)
        {

            if (word[i] == '>')
            {

                char *p = strtok_r(word, ">", &file);
                file = trim(file);

                flag = 1;
                break;
            }else if (word[i] == '<'){
                char *p = strtok_r(word, "<", &file);
                file = trim(file);

                flag = 2;
                break;
            }
            else if (word[i] == '|'){
                char *p = strtok_r(word, "|", &left);
                flag = 3;
                break;
            }
        }
        if (strcmp(word, "exit") == 0){
            exit(0);
        }

        if (flag == 1){
            parse(word, argv);
            execute_file(argv, file);
        }else if (flag == 2){
            parse(word, argv);
            execute_input(argv, file);
        }else if (flag == 3){
            char *argp[64];
            char *output, *file;
            if (strstr(left, "|") > 0)
            {
                char *p = strtok_r(left, "|", &file);
                parse(word, argv);
                parse(left, args);
                parse(file, argp);
                execute_pipe2(argv, args, argp);
            }else{
                parse(word, argv);
                execute_pipe(argv, left);
            }
        }else{
            parse(word, argv);
            execute(argv);
        }
    }

}