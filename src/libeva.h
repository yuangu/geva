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
#include "login.h"
#include "evadef.h"
#include "evasocket.h"

#include "package.h"
#include <pthread.h>

#ifdef __WIN32__
#define Import   __declspec( dllimport )
#define Export   __declspec( dllexport )
#elif defined(__LCC__) && defined(__WIN32__) /* LCC-Win32 */
#define Export __stdcall
#else
#define Import
#define Export
#endif

/* APIENTRY */
#if !defined(APIENTRY)
#  if defined(__WIN32__)
#    define APIENTRY __stdcall
#  else
#    define APIENTRY
#  endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
*@brief eva初使化
*@param <*eva><一个EVA型指针>  <user><用户帐号>  <passwd><用户密码><sta>  <见enum EVASTATUS(evadef.h)>
*@retrun <void>
*/
Export void  APIENTRY eva_init(EVA *eva);
/**
*@brief libeva版本获取
*@retrun <C串>
*/
Export char* APIENTRY eva_get_version();
/**
*@brief eva登录
*@retrun <登录错误，详情见enum LOGIN_ERRO（login.h）>
*/
Export int APIENTRY eva_login(EVA* eva,
              const char* user_char,char* passwd,status sta,
              char*  serverlist, int protocol
             );
/**
@brief 登录后发送的第一个数据包
*/
int eva_ad_012c(EVA *eva);
#ifdef __cplusplus
}
#endif
#endif
