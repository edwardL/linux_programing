#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<sys/file.h>
#include<wait.h>
#include<sys/mman.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

#define COUNT 100
#define PATHNAME "/etc/passwd"

int do_child(int proj_id)
{
    int interval;
    int *shm_p, shm_id;
    key_t shm_key;
    if((shm_key = ftok(PATHNAME,proj_id)) == -1)
    {
        perror("ftok()");
        exit(1);
    }
    shm_id = shmget(shm_key,sizeof(int),0);
    if(shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    shm_p = (int*)shmat(shm_id,NULL,0);

    /* critical section */
    interval = *shm_p;
    interval++;
    usleep(1);
    *shm_p = interval;

    if(shmdt(shm_p) < 0)
    {
        perror("shm_p");
        exit(1);
    }
    exit(0);
}

int main()
{
    pid_t pid;
    int count;
    int *shm_p;
    int shm_id, proj_id;
    key_t shm_key;

    proj_id = 1234;

    if((shm_id = ftok(PATHNAME,proj_id)) == -1)
    {
        perror("ftok()");
        exit(1);
    }

    shm_id = shmget(shm_key,sizeof(int),IPC_CREAT|IPC_EXCL|0600);
    if(shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }
    shm_p = (int*)shmat(shm_id,NULL,0);
    if((void*)shm_p ==(void*)-1) {
        perror("shmat");
        exit(-1);
    }
    *shm_p = 0;

    for(count = 0; count < COUNT; count++)
    {
        pid = fork();
        if(pid < 0) {
            perror("fork");
            exit(1);
        }
        if(pid == 0)
        {
            do_child(proj_id);
        }
    }

    for(count = 0; count < COUNT; count++)
    {
        wait(NULL);
    }

    if(shmdt(shm_p) < 0) {
        perror("shmdt");
        exit(1);
    }

    //shanchu gongxiang neicun 
    if(shmctl(shm_id,IPC_RMID,NULL) < 0)
    {
        perror("shmctl");
        exit(1);
    }
    exit(0);
}