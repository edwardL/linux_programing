#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>

#define BUF_4K 4096
#define BUF_8K 8 * 1024
#define BUF_12K 12 * 1024

int main()
{
    char a[BUF_4K];
    char b[BUF_8K];
    char c[BUF_12K];

    memset(a,'A',sizeof(a));
    memset(b,'B',sizeof(b));
    memset(c,'C',sizeof(c));

    int pipe_fd[2];
    int ret = pipe(pipe_fd);
    if(ret == -1)
    {
        fprintf(stderr,"failed to create pipe (%s)\n",strerror(errno));
        return 1;
    }
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        close(pipe_fd[0]);
        int loop = 0;
        while(loop++ < 10)
        {
            ret = write(pipe_fd[1],a,sizeof(a));
            printf("apid=%d write %d bytes to pipe\n",getpid(),ret);
        }
        exit(0);
    }

    pid = fork();
    if(pid == 0)
    {
        close(pipe_fd[0]);
        int loop = 0;
        while(loop++ < 10)
        {
            int ret = write(pipe_fd[1],b,sizeof(b));
            printf("bpid=%d write %d bytes to pipe\n",getpid(),ret);
        }
        exit(0);
    }

    pid = fork();
    if(pid == 0)
    {
        close(pipe_fd[0]);
        int loop = 0;
        while(loop++ < 10)
        {
            int ret = write(pipe_fd[1],c,sizeof(c));
            printf("cpid=%d write %d bytes to pipe\n",getpid(),ret);
        }
        exit(0);
    }

    close(pipe_fd[1]);
    sleep(1);

    int fd = open("test.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buf[1024* 4] = {0};
    int n = 1;
    while(1)
    {
        ret = read(pipe_fd[0],buf,sizeof(buf));
        if(ret == 0)
            break;
        printf("n=%02d pid = %d read %d bytes from pipe buf[4095] = %c\n",n++,getpid(),ret,buf[4095]);
        write(fd,buf,ret);
    }
    return 0;
}