
#include "socket.h"


int socket_create(int protocol,char* ip, unsigned short port){

#ifdef WIN32
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	WSAStartup(sockVersion, &wsaData);
#endif
 int fd;
 struct sockaddr_in addr;
 memset( &addr, 0, sizeof(struct sockaddr_in) );
 addr.sin_family=AF_INET;

 if( ip )
	addr.sin_addr.s_addr = inet_addr( ip );
 else
	addr.sin_addr.s_addr = INADDR_ANY;
 
 addr.sin_port = htons( port );

 if(protocol)   //udp
	fd=socket(AF_INET,SOCK_DGRAM,0);
 else //tcp
	fd=socket(AF_INET,SOCK_STREAM,0); 
 if(fd==-1){
#ifdef WIN32
	 WSACleanup();
#endif
       	log_message("socket create failed;");
 	return fd;
 }
 int port1=port;
while( bind( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) < 0)
 {
	  addr.sin_port = htons( ++port1 );
	 
 }
log_message("socket(id:%d) created  and binded : %s:%d ;",fd,inet_ntoa(addr.sin_addr),port);

 return fd;
}

//close a socket
void qqsocket_close( int fd )
{
#ifdef WIN32
	closesocket( fd );
#else
	close( fd );
#endif
log_message("colse the socket(id:%d)",fd);
} 

//connect a socket to remote server
int socket_connect(int fd,char* ip,unsigned short port)
{
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	if( (addr.sin_addr.s_addr = inet_addr( ip ) ) == -1 )
	{	struct hostent *host;
		host = gethostbyname( ip );
		if( host )
		{
			addr.sin_addr.s_addr = *(size_t*) host->h_addr_list[0];
			
		}else{
			return -1;
		}

  	
	}
       addr.sin_port = htons( port );
	if( connect( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0 )
	{
		return -1;
	}
	return 0;

}



int socket_send( int fd, char* buf, size_t size )
{
	int ret;
	size_t rest;
	rest = size;
	while( rest > 0 )
	{
		ret = send( fd, (char*)buf, rest, 0);
		if(ret <= 0 )
		{
			return ret;
		}
		rest -= ret;
		buf += ret;
	}
	return size;
}

int socket_recv( int fd, char* buf, size_t size )
{
	int ret;
	ret = recv( fd, buf, size, 0 );
	return ret;
}




