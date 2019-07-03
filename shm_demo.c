#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>

#define MYKEY 0x3333

int main()
{
    int shmid;
    void *ptr = NULL;
    shmid = shmget(MYKEY,4096,IPC_CREAT | IPC_EXCL | 0640);
    if(shmid == -1)
    {
        if(errno != EEXIST)
        {
            fprintf(stderr,"shmget returned %d (%d: %s)\n",shmid,errno,strerror(errno));
            return 1;
        }
        else
        {
            shmid = shmget(MYKEY,4096,0);
            if(shmid == -1)
            {
                fprintf(stderr,"shmget returned %d (%d: %s)\n",shmid,errno,strerror(errno));
                return 2;
            }
        }
    }
    fprintf(stdout,"shmid = %d\n",shmid);

    ptr = shmat(shmid,NULL,SHM_RND);
    if(ptr == (void*)-1)
    {
        fprintf(stderr,"shmat returned NULL, error (%d:%s)",errno,strerror(errno));
        return 2;
    }
    fprintf(stdout,"shmat return %p\n",ptr);

    sleep(1000);

    shmdt(ptr);

    return 0;
}