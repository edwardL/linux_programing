#include<mqueue.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"usage mqunlink <name>\n");
        return -1;
    }
    int ret = mq_unlink(argv[1]);
    if(ret != 0)
    {
        fprintf(stderr,"mq_unlink failed (%s)\n",strerror(errno));
        return -2;
    }

    return 0;
}
