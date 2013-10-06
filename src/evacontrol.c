#include "evacontrol.h"
#include "evadef.h"
#include "package.h"
#include "evacrypt.h"
#include <string.h>
#include <stdio.h>
#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif
void initcontrol(){
    int i;
    sendqueue.elem=malloc(sizeof(evabyte)*MAX_MESSAGE);
    recvqueue.elem=malloc(sizeof(evabyte)*MAX_MESSAGE);
    for(i=0; i<MAX_MESSAGE; i++)
    {
        initevabyte(sendqueue.elem+i,1024);
        initevabyte(recvqueue.elem+i,1024);
    }
    sendqueue.rear=sendqueue.front=recvqueue.rear=recvqueue.front=0;
}
void freecontrol(){
    int i;
    for(i=0; i<MAX_MESSAGE; i++)
    {
        freeevabyte(sendqueue.elem+i);
        freeevabyte(recvqueue.elem+i);
        }
    free(sendqueue.elem);
    free(sendqueue.elem);
}

void messagesend(EVA* eva)
{
    if(sendqueue.front==sendqueue.rear){
    #ifdef __WIN32__
    Sleep(1000);
    #else
    sleep(1);
    #endif
    }else{
    if(eva_net_send(eva->net,(sendqueue.elem+sendqueue.front))>0)
        sendqueue.front++;
    if(sendqueue.front==MAX_MESSAGE) sendqueue.front-=MAX_MESSAGE;
    }
}
void messagerecv(EVA* eva)
{
if(recvqueue.front==(recvqueue.rear+1)){
    #ifdef __WIN32__
    Sleep(1000);
    #else
    sleep(1);
    #endif
    }else{
    if(eva_net_recv(eva->net,(recvqueue.elem+recvqueue.rear))>0)
        sendqueue.rear++;
    if(sendqueue.rear==MAX_MESSAGE) sendqueue.rear-=MAX_MESSAGE;
    }

}

evabyte* pack_sessionkey(EVA *eva,evabyte* abyte,uchar *cmd,uchar *data,int len)
{
            abyte->len=0;

            uchar two[1]= {0x02};
            putevabyte(abyte,two,1);
            putevabyte(abyte,(eva->data)->version,2);
            putevabyte(abyte,cmd,2);

            seqadd((eva->data)->seq);

            putevabyte(abyte,(eva->data)->seq,2);
            putevabyte(abyte,(eva->user)->id,4);
            putevabyte(abyte,(eva->data)->gd,11);

            uchar outstr[1024];
            int outlen;
            qqencrypt( data,len,(eva->user)->seesionkey,outstr,&outlen);
            putevabyte(abyte,outstr,outlen);
            uchar three[1]= {0x03};
            putevabyte(abyte,three,1);


            return abyte;

}

evabyte* unpack_sessionkey(EVA* eva,evabyte* in,evabyte* out)
{
    uchar outstr[1024];
    int outlen;
    if(qqdecrypt((in->data+14),((in->len)-15),(eva->user)->seesionkey,outstr,&outlen)!=1)
    {
        printf("\n%X\n",*(in->data+4));
        printf("解密错误");
    }
    putevabyte(out,outstr,outlen);

    return out;

}

