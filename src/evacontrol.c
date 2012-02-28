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
evabyte* unpack_sessionkey(EVA* eva,evabyte* in,evabyte* out);
void messagesend(EVA* eva){
    int time=0;
    while(1){
        if((eva->sendmessage)->flag==1){
            if(eva_net_send(eva->net,(eva->sendmessage)->data)>0){
               (eva->sendmessage)->flag=0;
                time=0;
            }
            else{
            time++;
            if(time%3==0){  //这样就不会卡住了
                (eva->sendmessage)->flag=0;
                time=0;
                }
            }
        }else{

    #ifdef __WIN32__
        Sleep(1000);
    #else
        sleep(1);
    #endif
        }
    }
}
void messagerecv(EVA* eva){
    int time=0;
    evabyte recv;
    uchar data[1024];
    recv.data=data;
while(1){
        if((eva->recvmessage)->flag==1){
            memset(recv.data,0,1024);
            recv.len=0;
            if(eva_net_recv(eva->net,&recv)>0){
            //存放包命令及包序
            memcpy((eva->recvmessage)->cmd,recv.data+3,2);
            memcpy((eva->recvmessage)->seq,recv.data+5,2);

            unpack_sessionkey(eva,&recv,(eva->recvmessage)->data);
            (eva->recvmessage)->flag=0;
            time=0;
            }
            else{
                time++;
                if(time%3==0){
                (eva->recvmessage)->flag=0;
                time=0;
                }
            }
        }else{
    #ifdef __WIN32__
        Sleep(1000);
    #else
        sleep(1);
    #endif
        }
    }
}
//pcak with sessionkey
evabyte* pack_sessionkey(EVA* eva,uchar *cmd,uchar *data,int len){
    while(1)
    {if((eva->sendmessage)->flag==0){
    ((eva->sendmessage)->data)->len=0;
    uchar two[1]={0x02};
    putevabyte((eva->sendmessage)->data,two,1);
    putevabyte((eva->sendmessage)->data,(eva->data)->version,2);
    putevabyte((eva->sendmessage)->data,cmd,2);

    memcpy((eva->sendmessage)->cmd,cmd,2);

    seqadd((eva->data)->seq);
    memcpy((eva->sendmessage)->seq,(eva->data)->seq,2);

    memcpy((eva->sendmessage)->seq,((eva->data)->seq),2);
    putevabyte((eva->sendmessage)->data,(eva->data)->seq,2);
    putevabyte((eva->sendmessage)->data,(eva->user)->id,4);
    putevabyte((eva->sendmessage)->data,(eva->data)->gd,11);

    uchar outstr[1024];
    int outlen;
    qqencrypt( data,len,(eva->user)->seesionkey,outstr,&outlen);
    putevabyte((eva->sendmessage)->data,outstr,outlen);
    uchar three[1]={0x03};
    putevabyte((eva->sendmessage)->data,three,1);
    (eva->sendmessage)->flag=1;
    }
    else{
        #ifdef __WIN32__
        Sleep(1000);
        #else
        sleep(1);
        #endif
    }
    }
    return (eva->sendmessage)->data;

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

