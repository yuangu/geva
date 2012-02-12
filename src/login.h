#ifndef _LOGIN_H_
#define _LOGIN_H_
#include "evadef.h"
#include "libeva.h"

typedef struct EVAloindata{
    uchar EVA_91_FIX_DATA01[6];
    uchar EVA_91_FIX_DATA02[12];

    uchar EVA_LOGIN_TIME[4];
    uchar EVA_LOGIN_IP[4];
    uchar EVA_LOGIN_91_TOKEN[58];   /// first and two bytes is token of len

    uchar EVA_LOGIN_BA_TOKEN[58];

    uchar EVA_LOGIN_DD_RECV_KEY[16];
    uchar EVA_LOGIN_DD_TOKEN01[34];
    uchar EVA_LOGIN_DD_TOKEN02[154];   //包括len，时间还有token2等信息
    uchar EVA_LOGIN_DD_TOKEN03[58];
    uchar EVA_LOGIN_DD_TOKEN_key1[16];
    uchar EVA_LOGIN_DD_TOKEN_key2[16];

    uchar EVA_LOGIN_E5_TOKEN_key1[16];
    uchar EVA_LOGIN_E5_TOKEN_key2[16];
    uchar EVA_LOGIN_E5_FIX[4];
    uchar EVA_LOGIN_E5_DATA01[210];
    uchar EVA_LOGIN_E5_DATA02[123];
}LONDATA;
enum LOGIN_ERRO{
    NET_ERRO=0,   ///网络错误
    PASSWD_ERRO=-1, ///密码错误
    ID_ERRO=-2,  ///没有该账号
    NO_ERRO=-3, ///不允许登录


};
//LONDATA *logindata;
LONDATA* eva_login_init();
int eva_login_touch_send(EVA *eva,LONDATA* logindata);
int eva_login_touch_recv(EVA *eva,LONDATA* logindata);

int eva_login_request_send(EVA *eva,LONDATA* logindata);
int eva_login_request_recv(EVA *eva,LONDATA* logindata);
#endif
