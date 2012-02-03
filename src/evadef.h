#ifndef _QQDEF_H_
#define _QQDEF_H_

#define TCP 0
#define UDP 1
typedef unsigned char uchar;
//目前状态
 enum QQSTATUS{
	QQ_NONE = 0x00,
	QQ_ONLINE = 0x0a,
	QQ_OFFLINE = 0x14,
	QQ_AWAY = 0x1e,
	QQ_BUSY = 0x32,
	QQ_KILLME = 0x3C,
	QQ_QUIET = 0x46,
	QQ_HIDDEN = 0x28
};

typedef enum QQSTATUS status;
typedef struct {
	 uchar* data;
      	int len;

}evabyte;

//跟用户有关的内容
typedef struct {
	uchar id[4];
	uchar passwd_md5_one[16];
	uchar passwd_md5_two[16];
    	status sta;
	uchar seesionkey[16];
   
} evauser;
//跟客户端有关的内容
typedef struct {
	uchar version[2];  //版本
	uchar gd[16];    //固定的字节
	uchar seq[2];   //包序

} evadata;


#endif
