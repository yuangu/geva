#include "evasocket.h"
#include <stdlib.h>
#include <stdio.h>
//------------------------------------------------------------
int socket_create(int protocol,char* ip, unsigned short port)
{

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
    if(fd==-1)
    {
#ifdef WIN32
        WSACleanup();
#endif
//      	log_message("socket create failed;");
        return fd;
    }
// printf("%d",fd);
    int port1=port;
    int time=0;
    while( bind( fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) < 0)
    {
        time++;
        addr.sin_port = htons( ++port1 );
        if(time==20) return -1; //break;

    }
//log_message("socket(id:%d) created  and binded : %s:%d ;",fd,inet_ntoa(addr.sin_addr),port);

    return fd;
}
//----------------------------------------------------------------------------------
//close a socket
void eva_net_close( evanet* net)
{
#ifdef WIN32
    closesocket( net->fd );
#else
    close( net->fd );
#endif
//log_message("colse the socket(id:%d)",fd);
}

//connect a socket to remote server
int socket_connect(evanet* net)
{

    if( connect( net->fd, (struct sockaddr*)(net->remoteaddr), sizeof(struct sockaddr_in)) < 0 )
    {
        return -1;
    }
    return 0;

}
evanet*  eva_net_init(int protocol,char* localip, unsigned short localport,
                      char* remoteip, unsigned short remoteport)
{
    evanet *net;
    net=NULL;
    net=(evanet*)malloc(sizeof(evanet));
    memset(net,0,sizeof(evanet));

    int fd=socket_create(protocol,localip,localport);
    if (fd>0) net->fd=fd;
    else return NULL;
//    printf("fd:%d\n",fd);
    net->isudp=protocol;

    //struct sockaddr_in addr;
    net->remoteaddr=malloc(sizeof(struct sockaddr_in));
    memset((net->remoteaddr),0,sizeof(struct sockaddr_in));

    (net->remoteaddr)->sin_family=AF_INET;
    if( ((((net->remoteaddr)->sin_addr)).s_addr = inet_addr( remoteip ) ) == -1 )
    {
        struct hostent *host;
        host = gethostbyname( remoteip );
        if( host )
        {
            ((net->remoteaddr)->sin_addr).s_addr = *(size_t*) host->h_addr_list[0];

        }
        else
        {
            return NULL;
        }


    }
    (net->remoteaddr)->sin_port = htons( remoteport );



    if(protocol==TCP)
    {
        if(socket_connect(net)<0) return NULL;
    }
    return net;

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
int socket_sendto( char* buf, size_t size,evanet* net)
{


    int ret;
    size_t rest;
    rest = size;
    while( rest > 0 )
    {
        ret = sendto( net->fd, (char*)buf, rest,
                      0,
                      (const struct sockaddr*)(net->remoteaddr),
                      sizeof(struct sockaddr_in));
        if(ret <= 0 )
        {
            //printf("run sendto");
            return ret;
        }
        rest -= ret;
        buf += ret;
    }
    //printf("\nat sendto:%d",size);
    return size;

}
int socket_recvfrom( char* buf,size_t size,evanet* net )
{
    int ret;
    int addlen=sizeof(struct sockaddr_in);
    ret = recvfrom( net->fd, buf, size, 0 ,(struct sockaddr*)(net->remoteaddr),&addlen);
    return ret;
}
int  eva_net_send(evanet *net,evabyte* sendbyte)
{
    int sendlen;
    if(net->isudp==TCP)
        sendlen=socket_send( net->fd, (char*)(sendbyte->data), (size_t)(sendbyte->len));
    if(net->isudp==UDP){
        sendlen=socket_sendto((char*)(sendbyte->data),(size_t)(sendbyte->len),net);
    }
    return sendlen;
}
int eva_net_recv(evanet *net,evabyte* recvbyte){
    int recvlen;
    if(net->isudp==TCP)
    {recvlen=socket_recv(net->fd,(char*)recvbyte->data,MAX_LEN);
    recvbyte->len=recvlen;
    }
    if(net->isudp==UDP)
    {recvlen=socket_recvfrom((char*)recvbyte->data,MAX_LEN,net);
    recvbyte->len=recvlen;
    }
    return recvlen;
}

char* uchartoip(uchar* ipchar){
    //char dest[16];  //max len is 16
    char* dest=(char*)malloc(16*sizeof(char));
    memset(dest,0,16);
    char ipnum[3];   //max is 255

    memset(ipnum,0,3);
    sprintf(ipnum,"%d",*ipchar);
    printf("%s",ipnum);
    strcat(dest,ipnum);
    strcat(dest,".");


     memset(ipnum,0,3);
    sprintf(ipnum,"%d",*(ipchar+1));
    strcat(dest,ipnum);
    strcat(dest,".");


     memset(ipnum,0,3);
    sprintf(ipnum,"%d",*(ipchar+2));
    strcat(dest,ipnum);
    strcat(dest,".");

     memset(ipnum,0,3);
    sprintf(ipnum,"%d",*(ipchar+3));
    strcat(dest,ipnum);

    return dest;


}
