#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void msg_show_attr(int msg_id, struct msqid_ds msg_info)
{
    int ret = -1;
    sleep(1);
    ret = msgctl(msg_id,IPC_STAT,&msg_info);
    if(-1 == ret)
    {
        printf("failed to get message information\n");
        return;
    }
    printf("\n");
    printf("the bytes number of the queue:%d\n",msg_info.__msg_cbytes);
    printf("the message number of the queue:%d\n",msg_info.msg_qnum);
    printf("the max number bytes of the queue:%d\n",msg_info.msg_qbytes);
    printf("the last process to send the message:%d\n",msg_info.msg_lspid);
    printf("the last process to receive the message:%d\n",msg_info.msg_lrpid);
    printf("the last time to send the message:%d\n",msg_info.msg_stime);
    printf("the last time to receive the message:%d\n",msg_info.msg_rtime);
    printf("the last time to change the message:%d\n",msg_info.msg_ctime);
    printf("the UID of the message:%d\n",msg_info.msg_perm.uid);
    printf("the GID of the message:%d\n",msg_info.msg_perm.gid);
}

int main()
{
    int ret = -1;
    int msg_flags, msg_id;
    key_t key;
    struct msgmbuf{
        int mtype;
        char mtext[10];
    };
    struct msqid_ds msg_info;
    struct msgmbuf msg_mbuf;
    int msg_sflags, msg_rflags;
    char *msgpath = "/root/ipc/msg/";

    key = ftok(msgpath,'b');

    if(key != -1)
    {
        printf("create the key successfully\n");
    }
    else
    {
        printf("fail to create the key\n");
    }
    msg_flags = IPC_CREAT | IPC_EXCL;
    msg_id = msgget(key,msg_flags | 0x0666);

    if(-1 == msg_id)
    {
        printf("failed to build the message");
        return 0;
    }

    msg_show_attr(msg_id,msg_info);

    msg_sflags = IPC_NOWAIT;
    msg_mbuf.mtype = 10;
    memcpy(msg_mbuf.mtext,"test message gkh",sizeof("test message gkh"));
    ret = msgsnd(msg_id,&msg_mbuf,sizeof("test message gkh"),msg_flags);

    if(-1 == ret)
    {
        printf("failed to send the message\n");
    }

    msg_show_attr(msg_id,msg_info);

    msg_rflags = IPC_NOWAIT | MSG_NOERROR;

    ret = msgrcv(msg_id,&msg_mbuf,10,10,msg_rflags);

    if(-1 == ret)
    {
        printf("failed to receive the message\n");
    }
    else
    {
        printf("receive the message successfully\n");
        printf("the length of the message:%d\n",ret);
    }

    msg_show_attr(msg_id,msg_info);

msg_info.msg_perm.uid = 8;
msg_info.msg_perm.gid = 8;
msg_info.msg_qbytes = 12345;

ret = msgctl(msg_id, IPC_SET, &msg_info);

if(-1 == ret)
{
    printf("failed to set the message attribute\n");
    return 0;
}

msg_show_attr(msg_id,msg_info);

ret = msgctl(msg_id,IPC_RMID,NULL);

if(-1 == ret)
{
    printf("failed to delete the message\n");    
    return 0;
}

}