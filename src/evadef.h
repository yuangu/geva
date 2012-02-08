#ifndef _QQDEF_H_
#define _QQDEF_H_

#define TCP 0
#define UDP 1
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define EVA_VERSION	0x262F
#define EVA_GD  {0x02,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x64,0xDF}


#define  QQ_LOGIN_FIX_91_ONE   {0x00,0x00,0x08,0x04,0x01,0xE0}
#define  QQ_LOGIN_FIX_91_TWO   {0x00,0x00,0x03,0x61,0x00,0x00,0x00,0x01,0x00,0x00,0x0F,0x07}
#define  MAX_LEN  1024
//目前状态
 enum EVASTATUS{
	EVA_NONE = 0x00,
	EVA_ONLINE = 0x0a,
	EVA_OFFLINE = 0x14,
	EVA_AWAY = 0x1e,
	EVA_BUSY = 0x32,
	EVA_KILLME = 0x3C,
	EVA_QUIET = 0x46,
	EVA_HIDDEN = 0x28
};

typedef enum EVASTATUS status;
typedef struct {
	 uchar* data;
      	int len;

}evabyte;

//跟用户有关的内容
typedef struct {
	uchar id[4];
	uchar passwd_md5_one[20];
	uchar passwd_md5_two[16];
    	status sta;
	uchar seesionkey[16];

} evauser;
//跟客户端有关的内容
typedef struct {
	uchar version[2];  //版本
	uchar gd[11];    //固定的字节
	uchar seq[2];   //包序

} evadata;


#endif
