#include "command.h"

char *trim(char *string)
{
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

void parse(char *word, char **argv)
{
    int count = 0;
    memset(argv, 0, sizeof(char*)*(64));
    char *lefts = NULL;
    const char *split = " ";
    //处理字符
    while (1)
    {
        char *p = strtok_r(word, split, &lefts);
        if (p == NULL)
        {
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

        f = 1;
    }
}

void execute(char **argv)
{
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
void  execute_file(char **argv, char *output)
{
    pid_t pid;
    int status, flag;
    char *file = NULL;
    if ((pid = fork()) < 0)
    {
        printf("fork失败\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (strstr(output, ">")>0)
        {
            char *p = strtok_r(output, ">", &file);
            output += 1;
            file = trim(file);
            flag = 1;

            int old_stdout = dup(1);


            FILE *fp1 = freopen(output, "w+", stdout);

            execute_file(argv, file);
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
            exit(0);
        }
        if (strstr(output, "<") > 0)
        {
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            flag = 1;
            int fd = open(file, O_RDONLY);
            if (fd<0)
            {
                printf("没有此文件.");
                exit(0);
            }
        }
        if (strstr(output, "|")>0)
        {
            fflush(stdout); printf("here"); fflush(stdout);
            char *p = strtok_r(output, "|", &file);
            file = trim(file);
            flag = 1;

            char *args[64];
            parse(file, args);
            execute(args);
        }
        int old_stdout = dup(1);
        FILE *fp1 = freopen(output, "w+", stdout);
        if (execvp(argv[0], argv) < 0)
            printf("exec错误");
        fclose(stdout);
        FILE *fp2 = fdopen(old_stdout, "w");
        *stdout = *fp2;
        exit(0);
    }
    else
    {
        while (wait(&status) != pid)
            ;
    }
}
void  execute_input(char **argv, char *output)
{
    pid_t pid;
    int fd;
    char *file;
    int flag = 0;
    int status;
    if ((pid = fork()) < 0)
    {
        printf("fork失败\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (strstr(output, "<")>0)
        {
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            flag = 1;

            fd = open(output, O_RDONLY);
            if (fd<0)
            {
                printf("没有此文件");
                exit(0);
            }
            output = file;
        }
        if (strstr(output, ">")>0)
        {
            char *p = strtok_r(output, ">", &file);
            file = trim(file);
            flag = 1;
            fflush(stdout);

            fflush(stdout);
            int old_stdout = dup(1);
            FILE *fp1 = freopen(file, "w+", stdout);
            execute_input(argv, output);
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
            exit(0);
        }
        if (strstr(output, "|") > 0)
        {
            char *p = strtok_r(output, "|", &file);
            file = trim(file);
            flag = 1;
            char *args[64];
            parse(file, args);
            int pfds[2];
            pid_t pid, pid2;
            int status, status2;
            pipe(pfds);
            int fl = 0;
            if ((pid = fork()) < 0)
            {
                printf("fork失败\n");
                exit(1);
            }
            if ((pid2 = fork()) < 0)
            {
                printf("fork失败\n");
                exit(1);
            }
            if (pid == 0 && pid2 != 0)
            {
                close(1);
                dup(pfds[1]);
                close(pfds[0]);
                close(pfds[1]);
                fd = open(output, O_RDONLY);
                close(0);
                dup(fd);
                if (execvp(argv[0], argv) < 0)
                {
                    close(pfds[0]);
                    close(pfds[1]);
                    printf("exec错误");
                    fl = 1;
                    exit(0);
                }
                close(fd);
                exit(0);
            }
            else if (pid2 == 0 && pid != 0 && fl != 1)
            {
                close(0);
                dup(pfds[0]);
                close(pfds[1]);
                close(pfds[0]);
                if (execvp(args[0], args) < 0)
                {
                    close(pfds[0]);
                    close(pfds[1]);
                    printf("exec错误");
                    exit(0);
                }
            }
            else
            {
                close(pfds[0]);
                close(pfds[1]);
                while (wait(&status) != pid);
                while (wait(&status2) != pid2);
            }
            exit(0);
        }
        fd = open(output, O_RDONLY);
        close(0);
        dup(fd);
        if (execvp(argv[0], argv) < 0)
        {
            printf("exec错误");
        }
        close(fd);
        exit(0);
    }
    else
    {
        while (wait(&status) != pid);
    }

}

void execute_pipe(char **argv, char *output)
{
    int pfds[2], pf[2], flag;
    char *file;
    pid_t pid, pid2, pid3;
    int status, status2, old_stdout;
    pipe(pfds);
    //pfds[0]:读        pfds[1]:写
    int blah = 0;
    char *args[64];
    char *argp[64];
    int fl = 0;
    if ((pid = fork()) < 0)
    {
        printf("fork失败\n");
        exit(1);
    }
    if ((pid2 = fork()) < 0)
    {
        printf("fork失败\n");
        exit(1);
    }
    if (pid == 0 && pid2 != 0)
    {
        close(1);
        dup(pfds[1]);
        close(pfds[0]);
        close(pfds[1]);
        if (execvp(argv[0], argv) < 0)//run the command
        {
            close(pfds[0]);
            close(pfds[1]);
            printf("exec错误");
            fl = 1;
            kill(pid2, SIGUSR1);
            exit(0);
        }
    }
    else if (pid2 == 0 && pid != 0)
    {
        if (fl == 1){ exit(0); }
        if (strstr(output, "<") > 0)
        {
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            flag = 1;
            parse(output, args);
            execute_input(args, file);
            close(pfds[0]);
            close(pfds[1]);
            exit(0);
        }
        if (strstr(output, ">") > 0)
        {
            char *p = strtok_r(output, ">", &file);
            file = trim(file);
            flag = 1;

            parse(output, args);
            blah = 1;
        }

        else
        {
            parse(output, args);
        }
        close(0);
        dup(pfds[0]);
        close(pfds[1]);
        close(pfds[0]);
        if (blah == 1)
        {
            old_stdout = dup(1);
            FILE *fp1 = freopen(file, "w+", stdout);
        }
        if (execvp(args[0], args) < 0)
        {
            fflush(stdout);
            printf("exec错误 %d", pid);
            kill(pid, SIGUSR1);
            close(pfds[0]);
            close(pfds[1]);
        }
        fflush(stdout);
        printf("HERE");

        if (blah == 1)
        {
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
        }
    }
    else
    {
        close(pfds[0]);
        close(pfds[1]);
        while (wait(&status) != pid);
        while (wait(&status2) != pid2);
    }
}
void execute_pipe2(char **argv, char **args, char **argp)
{
    int status;
    int i;
    int pipes[4];
    pipe(pipes);
    pipe(pipes + 2);
    if (fork() == 0)
    {
        dup2(pipes[1], 1);
        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);
        if (execvp(argv[0], argv) < 0)
        {
            fflush(stdout);
            printf("exec错误");
            fflush(stdout);
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);
            exit(1);
        }
    }
    else
    {
        if (fork() == 0)
        {
            dup2(pipes[0], 0);
            dup2(pipes[3], 1);
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);
            if (execvp(args[0], args) < 0)
            {
                fflush(stdout);
                printf("exec错误");
                fflush(stdout);
                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);
                exit(1);
            }
        }
        else
        {
            if (fork() == 0)
            {
                dup2(pipes[2], 0);
                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);
                if (execvp(argp[0], argp) < 0)
                {
                    fflush(stdout);
                    printf("exec错误");
                    fflush(stdout);
                    close(pipes[0]);
                    close(pipes[1]);
                    close(pipes[2]);
                    close(pipes[3]);
                    exit(1);
                }
            }
        }
    }
    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);
    for (i = 0; i < 3; i++)
        wait(&status);
}