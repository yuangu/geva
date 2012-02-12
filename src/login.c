#include "login.h"
#include "evasocket.h"
#include "package.h"
#include "evacrypt.h"
#include <stdio.h>
/*create a rand key*/
static int rand2()
{
    return (rand()<<16)+rand();
}
static void randkey(uchar* key)
{
    memset(key,0,16);
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
    uchar outstr[1024];
    int outlen;
    qqencrypt( in->data,in->len,key,outstr,&outlen);
    putevabyte(out,outstr,outlen);
    uchar three[1]={0x03};
    putevabyte(out,three,1);
    return out;

}
evabyte* pack2(evabyte *in,evabyte *out,uchar* cmd,uchar* key,EVA* aeva,LONDATA* logindata){
    uchar two[1]={0x02};
    putevabyte(out,two,1);
    putevabyte(out,(aeva->data)->version,2);
    putevabyte(out,cmd,2);
    putevabyte(out,(aeva->data)->seq,2);
    putevabyte(out,(aeva->user)->id,4);
    putevabyte(out,(aeva->data)->gd,11);
    putevabyte(out,logindata->EVA_LOGIN_DD_TOKEN03,58);
    uchar outstr[1024];
    int outlen;
    qqencrypt( in->data,in->len,key,outstr,&outlen);
    putevabyte(out,outstr,outlen);
    uchar three[1]={0x03};
    putevabyte(out,three,1);
    return out;

}
evabyte* unpack(evabyte* in,evabyte *out,uchar* key){
    uchar outstr[1024];
    int outlen;
    delevabyte(in,14,(in->len-1));
    /*
    int i;
    for(i=0;i<in->len;i++)
        printf("%X ",*(in->data+i));
    */
    qqdecrypt(in->data,in->len,key,outstr,&outlen);
    putevabyte(out,outstr,outlen);
    return out;

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
int eva_login_touch_send(EVA *eva,LONDATA* logindata){

    uchar cmd[2]={0x00,0x91};
    randkey(key);

    evabyte decryptbyte;
    //evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,35);
    initevabyte(&sendbyte,150);

    uchar zeroone[2]={0x00,0x1};
    putevabyte(&decryptbyte,zeroone,2); //密文0-2个字节
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6); //3-8
    //printf("EVA_LOGIM_TOUCH");
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12); //9-20
    decryptbyte.len=35;//the last of 15 is zero
   // printf("%d",decryptbyte.len);
    int sendlen=eva_net_send(eva->net,pack(&decryptbyte,&sendbyte,cmd,key,eva));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte);
   // &decryptbyte=NULL;
    return sendlen;
}
int eva_login_touch_recv(EVA *eva,LONDATA* logindata){
    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,150);
    initevabyte(&encryptbyte,150);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
     if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,key);
    if(*(encryptbyte.data+75)==0x01){
        char* newip=uchartoip(encryptbyte.data+85);
        if( (((((eva->net)->remoteaddr)->sin_addr)).s_addr = inet_addr( newip ) ) == -1 ){
                return 0;
            }
        if ((eva->net)->isudp==0){
                if(socket_connect(eva->net)<0) return 0;


        }
        free (newip);
        eva_login_touch_send(eva,logindata);
        eva_login_touch_recv(eva,logindata);
    }
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_TIME,1,5);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_IP,5,9);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_91_TOKEN,17,75);

    int i;
    for(i=0;i<58;i++)
    {
        printf("%X ",*(logindata->EVA_LOGIN_91_TOKEN+i));
    }

    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
//0xba
int eva_login_request_send(EVA *eva,LONDATA* logindata){
    uchar cmd[2]={0x00,0xBA};
    randkey(key);
    seqadd((eva->data)->seq);
    evabyte decryptbyte;
    //evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,88);
    initevabyte(&sendbyte,150);

    uchar zeroone[2]={0x00,0x1};
    putevabyte(&decryptbyte,zeroone,2); //密文0-2个字节
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6); //3-8
    //printf("EVA_LOGIM_TOUCH");
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12); //9-20
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_91_TOKEN,58);//21-78
    uchar tail[10]={0x03,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    putevabyte(&decryptbyte,tail,10);//79-88
    int sendlen=eva_net_send(eva->net,pack(&decryptbyte,&sendbyte,cmd,key,eva));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte);
   // &decryptbyte=NULL;
    return sendlen;
}
int eva_login_request_recv(EVA *eva,LONDATA* logindata){
    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,150);
    initevabyte(&encryptbyte,150);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
    if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,key);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_BA_TOKEN,8,66);
    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
//0xdd
int eva_login_verify_send(EVA *eva,LONDATA* logindata){
     uchar cmd[2]={0x00,0xDD};
    randkey(key);
    seqadd((eva->data)->seq);
    evabyte decryptbyte;
    evabyte verifybyte;
    //evabyte encryptbyte;
    evabyte sendbyte;

    initevabyte(&decryptbyte,700);
    initevabyte(&sendbyte,700);
    initevabyte(&verifybyte,122);

    //the verify data
    uchar randbyte[4]={0x7D,0xC6,0xC0,0x86};
    putevabyte(&verifybyte,randbyte,4); //0-4
    verifybyte.len++; //5
    verifybyte.len++;*(verifybyte.data+5)=0x01; //6
    putevabyte(&verifybyte,(eva->user)->id,4); //7-10
    putevabyte(&verifybyte,logindata->EVA_91_FIX_DATA02,12); //11-22
    verifybyte.len+=3; //23-25
    putevabyte(&verifybyte,(eva->user)->passwd_md5_one,16); //26-41
    putevabyte(&verifybyte,logindata->EVA_LOGIN_TIME,4); //42-45
    verifybyte.len+=13;  //46-58
    putevabyte(&verifybyte,logindata->EVA_LOGIN_IP,4); //59-62
    verifybyte.len+=8; //63-70
    uchar gdfix[20]={0x00,0x10,0x85,0x6D,     0x43,0x47,0x0F,0xA7,
                     0xFC,0x45,0xAD,0x26,     0x12,0x6A,0x59,0x87,
                     0x6A,0x27};  //the first and two bytes is len
    putevabyte(&verifybyte,gdfix,20); //71-90
    randkey(logindata->EVA_LOGIN_DD_RECV_KEY);
    putevabyte(&verifybyte,logindata->EVA_LOGIN_DD_RECV_KEY,16); //90-105

    uchar outstr[150];
    int outlen;
    qqencrypt( verifybyte.data,verifybyte.len,(eva->user)->passwd_md5_two,outstr,&outlen);

    //the dd of uncypt datas
    uchar gdddfix[4]={0x00, 0xDE, 0x00 ,0x01, };
    putevabyte(&decryptbyte,gdddfix,4); //1-4
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6); //5-10
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12); //11-22
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_BA_TOKEN,58); //21-78
    decryptbyte.len+=2; *(decryptbyte.data+79)=outlen;   //79-80
    putevabyte(&decryptbyte,outstr,outlen); //81-200
    uchar noknowdata[87]={   0x00,0x14, //二十位数据key+crc32 前面为key后面crc32
                             0x9A,0xD8,0x3D,0x91,   0xBD,0x1B,0xD6,0x5D,
                             0x4E,0xB6,0x1E,0x2E,   0xEC,0xE5,0x3D,0x4B, //key
                             0xB6,0xB2,0xEF,0x4E,   //key of crc32
                             0x00,0x00,0x38,0x2C,   0x91,0x64,0x36,0xE2, //固定
                             0xDA,0x94,    //key of len[密钥长度]
                             0xFC,0x9B,0xDA,0xDC,   0x15,0x4B,0x9D,0x7E,
                             0x2C,0x68,0x0D,0xF9,   0x71,0x9C,0x08,0xC1, //根据QQ版本而固定的[QQ_LOGIN_FIX_DD_KEY1 后面要用
                             0x1A,0x20,0xB5,0xD5,   0x73, //填充（固定）
                             0xDC,0x08,   //[密钥长度]
                             0xFC,0x9B,0xDA,0xDC,   0x15,0x4B,0x9D,0x7E,
                             0x2C,0x68,0x0D,0xF9,   0x71,0x9C,0x08,0xC1,//[QQ_LOGIN_FIX_DD_KEY2后面要用]
                             0x70,0xC9,0x74,0xE4,   0xEC,0x8E,0x2F,0xAB,
                             0xA2,0xDB,0x01,0x3C,   0x2E,0x01,0x85,0xB7,
    };
    putevabyte(&decryptbyte,noknowdata,87); //201-287
    decryptbyte.len+=328; //the last of 328 zero
    int sendlen=eva_net_send(eva->net,pack(&decryptbyte,&sendbyte,cmd,key,eva));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte),freeevabyte(&verifybyte);
    return sendlen;
}
int eva_login_verify_recv(EVA *eva,LONDATA* logindata){
    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,350);
    initevabyte(&encryptbyte,350);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
     if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,logindata->EVA_LOGIN_DD_RECV_KEY);
    if(*(encryptbyte.data+3)==0x33||*(encryptbyte.data+3)==0x51) return NO_ERRO;
    if(*(encryptbyte.data+3)==0xBF) return ID_ERRO;
    if(*(encryptbyte.data+3)==0x34) return PASSWD_ERRO;
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN01,3,37);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN02,37,191);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN03,191,249);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN_key1,249,265);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN_key1,267,283);
    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
//0xE5
int eva_login_E5_send(EVA *eva,LONDATA* logindata){
    uchar cmd[2]={0x00,0xE5};
    seqadd((eva->data)->seq);
    evabyte decryptbyte;
    //evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,400);
    initevabyte(&sendbyte,400);
    uchar fixbyte[5]={0x01,0x0E,0x00,0x01,0x01};
    putevabyte(&decryptbyte,fixbyte,5);  //0-5
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6);
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12);
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_BA_TOKEN,58);
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_DD_TOKEN01,34);

    putevabyte(&decryptbyte,logindata->EVA_LOGIN_DD_TOKEN02,154);
    *(decryptbyte.data+decryptbyte.len+2)=0x01;
    decryptbyte.len+=7;

    int sendlen=eva_net_send(eva->net,pack2(&decryptbyte,&sendbyte,cmd,logindata->EVA_LOGIN_DD_TOKEN_key1,eva,logindata));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte);
    return sendlen;

}
int eva_login_E5_recv(EVA *eva,LONDATA* logindata){
    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,640);
    initevabyte(&encryptbyte,640);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
     if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN_key2);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_E5_TOKEN_key1,4,20);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_E5_FIX,28,32);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_E5_DATA01,48,258);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_E5_TOKEN_key2,258,274);
    getucharfromevabyte(&encryptbyte,logindata->EVA_LOGIN_E5_DATA01,274,397);
    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
//E3
int eva_login_E3_send(EVA *eva,LONDATA* logindata){
    uchar cmd[2]={0x00,0xE3};
    seqadd((eva->data)->seq);
    evabyte decryptbyte;
    //evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,400);
    initevabyte(&sendbyte,400);
    uchar fixbyte[4]={0x00, 0xC8, 0x00 ,0x01 };
    putevabyte(&decryptbyte,fixbyte,4);  //0-4
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6);
    putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12);
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_BA_TOKEN,58);
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_E5_DATA02,123);
    decryptbyte.len+=2;
    *(decryptbyte.data+decryptbyte.len+2-1)=0x20;
    decryptbyte.len+=2;
    decryptbyte.len+=0x20;

    int sendlen=eva_net_send(eva->net,pack2(&decryptbyte,&sendbyte,cmd,logindata->EVA_LOGIN_DD_TOKEN_key1,eva,logindata));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte);
    return sendlen;

}
int eva_login_E3_recv(EVA *eva,LONDATA* logindata){
    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,100);
    initevabyte(&encryptbyte,100);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
     if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,logindata->EVA_LOGIN_DD_TOKEN_key2);
    int nick_len=*(encryptbyte.data+7);
    getucharfromevabyte(&encryptbyte,(eva->user)->nickname,8,8+nick_len);


    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
//0x30
int eva_login_30_send(EVA *eva,LONDATA* logindata){
    uchar cmd[2]={0x00,0x30};
    seqadd((eva->data)->seq);
    evabyte decryptbyte;
    //evabyte encryptbyte;
    evabyte sendbyte;
    //密文
    initevabyte(&decryptbyte,900);
    initevabyte(&sendbyte,900);
    /*uchar fixbyte[5]={00,0x01 };
    putevabyte(&decryptbyte,fixbyte,5);  //0-5
    */
    decryptbyte.len+=2;
    *(decryptbyte.data+1)=0x01;
     putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA02,12);
     putevabyte(&decryptbyte,logindata->EVA_LOGIN_E5_FIX,4);
     putevabyte(&decryptbyte,logindata->EVA_LOGIN_TIME,4);
     putevabyte(&decryptbyte,logindata->EVA_LOGIN_IP,4);
     decryptbyte.len+=8;   // 8 of 00
     putevabyte(&decryptbyte,logindata->EVA_LOGIN_E5_DATA01,210);
     decryptbyte.len+=35;  //35 of 00
     uchar fixdata[17]={0xC0,0x4B,0x26,0xBC,   0x20,0x79,0xDB,0xA2,
                        0x7B,0xEB,0x2F,0x0D,    0x12,0x0C,0xE9,0xEA,  //固定的填充
                        0x96,//随机字节
     };
     putevabyte(&decryptbyte,fixdata,17);
     uchar EVAstauts[1];
     *(EVAstauts)=(uchar)((eva->user)->sta);
     putevabyte(&decryptbyte,EVAstauts,1);
     *(decryptbyte.data+decryptbyte.len+5)=0x01;
     decryptbyte.len+=16;
     decryptbyte.len+=9;
     putevabyte(&decryptbyte,logindata->EVA_91_FIX_DATA01,6);
     decryptbyte.len+=16;
     putevabyte(&decryptbyte,logindata->EVA_LOGIN_BA_TOKEN,58);
     uchar gdfix[37]={    0x00,0x00,0x00,0x08,    0x00,0x00,0x00,0x00,
                        0x08,0x04,0x10,0x00,    0x01,0x40,0x01,0x85,
                        0xB7,0x23,0x1A,0x00,    0x10,0xE4,0xF4,0x64,
                        0xF4,0xD7,0x2C,0x9E,    0xF1,0x85,0xFD,0x01,
                        0x3B,0x64,0x61,0x15,    0xDA,};
     putevabyte(&decryptbyte,gdfix,37);
     *(decryptbyte.data+decryptbyte.len+5)=0x01;
    decryptbyte.len+=25;
    uchar gd_dd[7]={0x02, 0x20, 0x82, 0x45, 0x7D ,  //DD包固定填充
    00 ,0x10 }; //len of the dd key01
    putevabyte(&decryptbyte,gd_dd,7);
    putevabyte(&decryptbyte,logindata->EVA_LOGIN_DD_TOKEN_key1,7);
     decryptbyte.len+=249;
     int sendlen=eva_net_send(eva->net,pack2(&decryptbyte,&sendbyte,cmd,logindata->EVA_LOGIN_DD_TOKEN_key1,eva,logindata));
    if(sendlen==0) return NET_ERRO;
    freeevabyte(&decryptbyte),freeevabyte(&sendbyte);
    return sendlen;
}
int eva_login_30_recv(EVA *eva,LONDATA* logindata){

    evabyte recvbyte;
    evabyte encryptbyte;
    initevabyte(&recvbyte,200);
    initevabyte(&encryptbyte,200);
    int recvlen=eva_net_recv(eva->net,&recvbyte);
     if(recvlen==0) return NET_ERRO;
    unpack(&recvbyte,&encryptbyte,logindata->EVA_LOGIN_E5_TOKEN_key1);
    if(encryptbyte.len==0) return NET_ERRO;
    if(*(encryptbyte.data)==0)
    {
        getucharfromevabyte(&encryptbyte,(eva->user)->seesionkey,1,17);
    }
    freeevabyte(&recvbyte),freeevabyte(&encryptbyte);
    return recvlen;
}
