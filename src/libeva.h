/**@file
*@brief libeva的接口
*@atuhor <元谷 lifulinghan@aol.com>
*@data <2012-2>
*@version <1.0.0>
*@note
*这是一个libeva的接口，通过此接口可以调用libeva的函数
*/
#ifndef _LIB_EVA_H_
#define _LIB_EVA_H_
#include "evadef.h"
#include "evasocket.h"
typedef struct{
	evanet* net;
	evauser* user;
	evadata* data;
} EVA;
/**
*@brief eva初使化
*@param <*eva><一个EVA型指针><user><用户帐号><passwd><用户密码><sta><是否隐身，0表示是隐身，1表示不隐身>
*@retrun <void>
*/
void eva_init(EVA *eva,ulong user,char* passwd,status sta);
#endif
