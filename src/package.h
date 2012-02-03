#ifndef _PACKAGE_H_
#define _PACKAGE_H_
#include "evadef.h"
//初使一个evabyte型
evabyte* initevabyte(evabyte* aevabyte,int maxlen);
//释放evabyte内存
void freeevabyte(evabyte *aevabyte);
//把无符号数据填充到evabyte
evabyte* putevabyte(evabyte* inevabyte,const uchar* str,int len);
//删除frist到last的数据
evabyte* delevabyte(evabyte* inevabyte,int frist,int last);
//将evabyte型转成uchar*,为了使功能更加强大，特加了一个frist,last的范围，当两值全为0//时就是将evabyte型全部转成uchar*
uchar* getucharfromevabyte(evabyte* inevabyte,uchar* out,int first,int last);
#endif
