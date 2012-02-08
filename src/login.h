#ifndef _LOGIN_H_
#define _LOGIN_H_
#include "evadef.h"
#include "libeva.h"
typedef struct EVAloingdata{
    uchar EVA_91_FIX_DATA01[6];
    uchar EVA_91_FIX_DATA02[12];

}LONDATA;
LONDATA *logindata;
#endif
