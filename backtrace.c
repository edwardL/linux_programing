#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<execinfo.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define PRINT_DEBUG

static void print_reason(int sig,siginfo_t* info, void* secret)
{
  void *array[10];
  size_t size;
  #ifdef PRINT_DEBUG
    char** strings;
    size_t i;
    size = backtrace(array,10);
    strings = backtrace_symbols(array,size);
    printf("obtained %zd stack frames.\n",size);
    for(i = 0; i < size; i++)
	printf("%s\n",strings[i]);
    free(strings);
#else
    int fd = open("err.log",O_CREAT|O_WRONLY);
    size = backtrace(array,10);
    backtrace_symbols_fd(array,size,fd);
    close(fd);
#endif
    exit(0);
}

void die()
{
    char* test1;
    char* test2;
    char* test3;
    char* test4 = NULL;
    strcpy(test4,"ab");
}

void test1()
{
    die();
}

int main(int argc,char** argv)
{
    struct sigaction myAction;
    myAction.sa_sigaction = print_reason;
    sigemptyset(&myAction.sa_mask);
    myAction.sa_flags = SA_RESTART | SA_SIGINFO;
    sigaction(SIGSEGV,&myAction,NULL);
    sigaction(SIGUSR1,&myAction,NULL);
    sigaction(SIGFPE,&myAction,NULL);
    sigaction(SIGILL,&myAction,NULL);
    sigaction(SIGBUS,&myAction,NULL);
    sigaction(SIGABRT,&myAction,NULL);
    sigaction(SIGSYS,&myAction,NULL);

    test1();
}
