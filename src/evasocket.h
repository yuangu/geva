#ifndef _EVA_SOCKET_H
#define _EVA_SOCKET_H

#include "evadef.h"
#include<string.h>
#include<sys/types.h>
#include <fcntl.h>
#ifdef __WIN32__
#include<winsock.h>
#include <wininet.h>
#else
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <netdb.h>
#endif

evanet*  eva_net_init(int protocol,char* localip, unsigned short localport,
                      char* remoteip, unsigned short remoteport);

void eva_net_close( evanet* net);
int socket_connect(evanet* net);
int eva_net_send(evanet *net,evabyte* sendbyte);
int eva_net_recv(evanet *net,evabyte* recvbyte);
char* uchartoip(uchar* ipchar);

#endif
