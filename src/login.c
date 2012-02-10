#include "login.h"
#include "socket.h"
#include "package.h"
#include <stdio.h>
/*create a rand key*/
static int rand2()
{
    return (rand()<<16)+rand();
}
static void randkey(uchar* key)
{
    int i;
    for( i=0; i<16; i++ )
        key[i] = rand2();
}
//pcak for login
evabyte* pack(evabyte *in,evabyte *out,uchar* cmd,uchar* key,EVA* aeva){
    uchar two[1]={0x02};
    putevabyte(out,two,1);
    putevabyte(out,(aeva->data)->version,2);
    putevabyte(out,cmd,2);
    putevabyte(out,(aeva->data)->seq,2);
    putevabyte(out,(aeva->user)->id,4);
    putevabyte(out,(aeva->data)->gd,11);
    putevabyte(out,key,16);
    putevabyte(out,in->data,in->len);
    uchar three[1]={0x03};
    putevabyte(out,three,1);
    return in;

}
uchar key[16];
LONDATA* eva_login_init(){
    LONDATA* logindata;
    logindata=(LONDATA*)malloc(sizeof(LONDATA));
    uchar fix1[6]=QQ_LOGIN_FIX_91_ONE;
    memcpy(logindata->EVA_91_FIX_DATA01,fix1,6);
    uchar fix2[12]=QQ_LOGIN_FIX_91_TWO;
    memcpy(logindata->EVA_91_FIX_DATA02,fix2,12);

    return logindata;
}
int eva_login_touch(EVA *eva,LONDATA* logindata){

    uchar cmd[2]={0x00,0x91};
    randkey(key);

    evabyte decryptbyte;
    evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,35);
    initevabyte(&encryptbyte,80);
    initevabyte(&sendbyte,150);

    uchar zeroone[2]={0x00,0x1};
    putevabyte(&decryptbyte,zeroone,2); //密文0-2个字节
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6); //3-8
    //printf("EVA_LOGIM_TOUCH");
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12); //9-20
    decryptbyte.len=35;//the last of 15 is zero
   // printf("%d",decryptbyte.len);
    int sendlen=eva_net_send(eva->net,pack(&encryptbyte,&sendbyte,key,cmd,eva));

    int i;
    for(i=0;i<sendbyte.len;i++)
    printf("%X ",*(sendbyte.data+i));

    freeevabyte(&decryptbyte),freeevabyte(&encryptbyte);
    return sendlen;
}

