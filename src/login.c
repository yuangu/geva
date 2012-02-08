#include "login.h"
#include "socket.h"
#include "package.h"

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
uchar key[16];
void eva_login_init(){
    logindata=(LONDATA*)malloc(sizeof(LONDATA));
    uchar fix1[6]=QQ_LOGIN_FIX_91_ONE;
    memcpy(logindata->EVA_91_FIX_DATA01,fix1,6);
    uchar fix2[12]=QQ_LOGIN_FIX_91_TWO;
    memcpy(logindata->EVA_91_FIX_DATA02,fix2,12);

}
int eva_login_touch(EVA *eva){
    randkey(key);

    evabyte* decryptbyte=NULL;
    evabyte* encryptbyte=NULL;
    //密文
    initevabyte(decryptbyte,35);
    uchar zeroone[2]={0x00,0x1};
    putevabyte(decryptbyte,zeroone,2); //密文0-2个字节
    putevabyte(decryptbyte,logindata->EVA_91_FIX_DATA01,6); //3-8
    putevabyte(decryptbyte,logindata->EVA_91_FIX_DATA02,12); //9-20
    decryptbyte->len=35;//the last of 15 is zero
    //正文了
    encryptevabyte(decryptbyte,encryptbyte,key);
    socket_send( int fd, char* buf, size_t size );

    freeevabyte(evabyte *aevabyte)
    return 1;
}

