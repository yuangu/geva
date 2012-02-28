#ifndef EVACONTROL_H_INCLUDED
#define EVACONTROL_H_INCLUDED
#include "libeva.h"
void messagesend(EVA* eva);
void messagerecv(EVA* eva);
evabyte* pack_sessionkey(EVA* eva,uchar *cmd,uchar *data,int len);
#endif // EVACONTROL_H_INCLUDED
