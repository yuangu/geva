#ifndef EVACONTROL_H_INCLUDED
#define EVACONTROL_H_INCLUDED
#include "libeva.h"
typedef struct{
    evabyte *elem;
    int rear;
    int front;
}Queue;

 Queue sendqueue;
 Queue recvqueue;

void messagesend(EVA* eva);
void messagerecv(EVA* eva);
evabyte* pack_sessionkey(EVA *eva,evabyte* abyte,uchar *cmd,uchar *data,int len);
evabyte* unpack_sessionkey(EVA* eva,evabyte* in,evabyte* out);
#endif // EVACONTROL_H_INCLUDED
