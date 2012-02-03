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
int socket_create(int protocol,char* ip, unsigned short port);
void qqsocket_close( int fd );
int socket_connect(int fd,char* ip,unsigned short port);
int socket_send( int fd, char* buf, size_t size );
int socket_recv( int fd, char* buf, size_t size );
#endif
