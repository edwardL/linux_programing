#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include<signal.h>

#define MAX_LINE_SIZE 8192

void print_wait_exit(int status)
{
    printf("status = %s\n",status);
    if(WIFEXITED(status))
    {
        printf("normal termination,exit status = %d\n",WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status))
    {
        printf("abnormal terminition, signal number = %d%s\n",WTERMSIG(status),
        #ifdef WCOREDUMP
            __WCOREDUMP(status) ? "core file generated" : "");
        #else
            "");
        #endif
    }
}

int main(int argc, char const *argv[])
{
    FILE* fp = NULL;
    char command[MAX_LINE_SIZE], buffer[MAX_LINE_SIZE];
    if(argc != 2)
    {
        fprintf(stderr,"usage: %s filename \n",argv[0]);
        exit(1);
    }

    snprintf(command,sizeof(command),"cat %s",argv[1]);
    fp = popen(command,"r");
    if(fp == NULL)
    {
        fprintf(stderr,"popen failed %s",strerror(errno));
        exit(2);
    }
    while(fgets(buffer,MAX_LINE_SIZE,fp) != NULL)
    {
        fprintf(stdout,"%s",buffer);
    }
    int ret = pclose(fp);
    if(ret == 127)
    {
        fprintf(stderr,"bad command : %s\n",command);
        exit(3);
    }
    else if(ret == -1)
    {
        fprintf(stderr,"failed to get child status (%s)\n",strerror(errno));
        exit(4);
    } else
    {
        print_wait_exit(ret);
    }
    return 0;
}
