#ifndef SWIFT_SOCKET_H
#define SWIFT_SOCKET_H
#include "evadef.h"
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include <fcntl.h>
#ifdef __WIN32__
#include<winsock.h>
#include <wininet.h>
#else
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <netdb.h>
#endif

typedef struct _eavnet_{
    int fd;
    int isudp;
    struct sockaddr_in remoteaddr;
}evanet;

evanet*  eva_net_init(int protocol,char* localip, unsigned short localport,
                      char* remoteip, unsigned short remoteport);

void eva_net_close( evanet* net);
int eva_net_send(evanet *net,evabyte* sendbyte);
int eva_net_recv(evanet *net,evabyte* recvbyte);
#endif
