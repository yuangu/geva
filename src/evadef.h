#ifndef _QQDEF_H_
#define _QQDEF_H_
#include<pthread.h>
/* __WIN32__ */
#if !defined(__WIN32__) && (defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__))
#  define __WIN32__
#endif

#define TCP 0
#define UDP 1

#define EVA_VERSION	0x262F
#define EVA_GD  {0x02,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x64,0xDF}


#define  QQ_LOGIN_FIX_91_ONE   {0x00,0x00,0x08,0x04,0x01,0xE0}
#define  QQ_LOGIN_FIX_91_TWO   {0x00,0x00,0x03,0x61,0x00,0x00,0x00,0x01,0x00,0x00,0x0F,0x07}
#define  MAX_LEN  1024



typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef void*(*CALLFUNC)(void*);

//目前状态
enum EVASTATUS{
	EVA_NONE = 0x00,
	EVA_ONLINE = 0x0a,
	EVA_OFFLINE = 0x14,
	EVA_AWAY = 0x1e,
	EVA_BUSY = 0x32,
	EVA_KILLME = 0x3C,
	EVA_QUIET = 0x46,
    EVA_HIDDEN=0x28,
};

typedef enum EVASTATUS status;
typedef struct {
	 uchar* data;
      	int len;

}evabyte;

//跟用户有关的内容
typedef struct {
	uchar id[4];
	uchar passwd_md5_one[16];
	uchar passwd_md5_two[16];
	uchar nickname[100];
    	status sta;
	uchar seesionkey[16];

} evauser;
//跟客户端有关的内容
typedef struct {
	uchar version[2];  //版本
	uchar gd[11];    //固定的字节
	uchar seq[2];   //包序
} evadata;
typedef struct{
    uchar flag;
    uchar cmd[2];
    uchar seq[2];
    evabyte* data;
}evamessage;

typedef struct _eavnet_{
    int fd;
    int isudp;
    struct sockaddr_in* remoteaddr;
}evanet;

typedef struct{
	evanet* net;
	evauser* user;
	evadata* data;
    evamessage *sendmessage;
    evamessage *recvmessage;
    pthread_t* send;
    pthread_t* recv;
} EVA;
#endif
