#include<mqueue.h>
#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

int main(int argc, char *argv[])
{
    int c, flags;
    mqd_t mqd;
    flags = O_RDWR | O_CREAT;
    while((c = getopt(argc,argv,"e")) != -1)
    {
        switch(c)
        {
            case 'e':
                flags |= O_EXCL;
                break;
        }
    }
    if(optind != argc - 1)
    {
        fprintf(stderr,"usage:mqcreate [-e] <name>\n");
        return -1;
    }

    mqd = mq_open(argv[optind],flags,S_IRUSR | S_IWUSR ,NULL);
    if(mqd == -1)
    {
        fprintf(stderr,"mq_open failed (%s)\n",strerror(errno));
        return -2;
    }
    mq_close(mqd);

    return 0;
}
