#include"libeva.h"
#include "login.h"
#include "evacontrol.h"
#include "evaad.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#include "md5.h"

Export void  APIENTRY eva_init(EVA *eva)
{
    /*create eva*/
    eva->user=malloc(sizeof(evauser));
    memset(eva->user,0,sizeof(evauser));
    eva->data=malloc(sizeof(evadata));
    memset(eva->data,0,sizeof(evadata));

    /*init eva of version data*/
    uint version=htons(EVA_VERSION);
    memcpy((eva->data)->version,&version,2);
    uchar gd[11]=EVA_GD;
    memcpy((eva->data)->gd,&gd,11);
//seq is zero,so not init now


}
char* eva_get_version()
{
    char* version;
    version=malloc(15);
    memset(version,0,15);
    memcpy(version,"libeva 0.0.1",strlen("libeva 0.0.1"));
    return version;

}
Export int APIENTRY eva_login(EVA* eva,
              const char* user_char,char* passwd,status sta,
              char*  serverlist, int protocol
             )
{
    int i;
    int flag=1;
    char *swap;
    char server[16];
    unsigned short port;
    swap=serverlist;
    if(protocol==UDP) port=8000;
    else if(protocol==TCP)port=443;
    //init net
    eva->net=NULL;
    while(eva->net==NULL)
    {
        for(i=0; i<16; i++)
        {
            if(*swap=='|'||*swap=='\0')
            {
                *(server+i)='\0';
                if(*swap!='\0')
                    swap+=1;
                break;
            }
            else
            {
                *(server+i)=*swap;
                swap+=1;
            }
        }
        eva->net=eva_net_init(protocol,"0.0.0.0",4000,
                              server, port);  //网络初使化
        if(*swap=='\0') break;
    }

    if(eva->net==NULL)   return 0; //由net  init错误，放到这里来！


    //for user id
    unsigned long user;
    user=(unsigned long)strtol(user_char,NULL,10);
    //printf("%d",user);
    /*init user of data*/
    //for qq of id
    int user_id=htonl(user);
    memcpy((eva->user)->id,&user_id,4);

    //for passwd
    uchar md5_pass[16];

    md5_state_t mst;
    md5_init( &mst );
    md5_append( &mst, (md5_byte_t*)passwd, strlen(passwd));
    md5_finish( &mst, (md5_byte_t*)md5_pass );

    memcpy( (eva->user)->passwd_md5_one, md5_pass, 16 );

    uchar md51passwithid[24];
    memset(md51passwithid,0,24);
    memcpy(md51passwithid, md5_pass, 16 );

    for(i=0; i<4; i++)
    {
        *(md51passwithid+20+i)=*((eva->user)->id+i);
    }

    md5_init( &mst );
    md5_append( &mst, (md5_byte_t*)md51passwithid,24);
    md5_finish( &mst, (md5_byte_t*)((eva->user)->passwd_md5_two) );

    //for status
    ((eva->user)->sta)=sta;
    LONDATA* logindata=eva_login_init();
    for(;;)
    {
        if(*swap=='\0') break;
        if(flag<=0)
        {
            while(1)
            {
                for(i=0; i<16; i++)
                {
                    if(*swap=='|'||*swap=='\0')
                    {
                        *(server+i)='\0';
                        if(*swap!='\0')swap+=1;
                        break;
                    }
                    else
                    {
                        *(server+i)=*swap;
                        swap+=1;
                    }
                }

                if( (((((eva->net)->remoteaddr)->sin_addr)).s_addr = inet_addr( server) ) == -1 )
                {
                    struct hostent *host;
                    host = gethostbyname( server );

                    if(host==NULL) continue;

                    if( host )
                    {
                        (((eva->net)->remoteaddr)->sin_addr).s_addr = *(size_t*) host->h_addr_list[0];
                        break;

                    }
                    else
                    {
                        continue;
                    }
                }
                else break;
                if(*swap=='\0') break;
            }

        }
        flag=eva_login_touch_send(eva,logindata);

        if(flag<=0) continue;
        flag=eva_login_touch_recv(eva,logindata);
        if(flag<=0) continue; //0x91
        flag=eva_login_request_send(eva,logindata);
        if(flag<=0) continue;
        flag=eva_login_request_recv(eva,logindata);
        if(flag<=0) continue;   //0xAB
        flag=eva_login_verify_send(eva,logindata);
        if(flag<=0)continue;
        flag=eva_login_verify_recv(eva,logindata);
        if(flag==0) continue;  //0xdd
        if(flag<0) return flag;
        flag=eva_login_E5_send(eva,logindata);
        if(flag<=0) continue;

        flag=eva_login_E5_recv(eva,logindata);
        if(flag<=0) continue; //E5
        flag=eva_login_E3_send(eva,logindata);
        if(flag<=0) continue;

        flag=eva_login_E3_recv(eva,logindata);
        if(flag<=0)continue;//E3
        flag=eva_login_30_send(eva,logindata);
        if(flag<=0) continue;
        flag=eva_login_30_recv(eva,logindata);
        if(flag<=0) continue;

        break;
    }
    free(logindata);
    if(flag<=0) return flag;
    else
    {

        pthread_t* send=malloc(sizeof(pthread_t));
        pthread_t* recv=malloc(sizeof(pthread_t));
        eva->send=send;
        eva->recv=recv;

        pthread_create(send,NULL,(CALLFUNC)(&messagesend),(void*)eva);
        pthread_create(recv,NULL,(CALLFUNC)(&messagerecv),(void*)eva);

        pthread_detach( *send );
        pthread_detach( *recv );

        return 1;
    }
}
Export int APIENTRY eva_ad_012c(EVA *eva){
return  eva_ad_012c_send(eva);
}

//debug
/*
debug
*/
//测试QQ号
//帐号       - 密码
//2386953608 - 0123456789
//2421723768 - 0123456789
//847708268  - testtest
//1348186323 -012345678
/*
int main(int agrc,char *argv[])
{
    printf("开始工作了！");
    EVA eva;    //申明一个EVA变量
    eva_init(&eva);  //EVA初使化
    int flag= eva_login(&eva,
                        "847708268","testtest",EVA_ONLINE,
                        "112.90.86.196|183.60.16.154|sz2.tencent.com|sz3.tencent.com|sz4.tencent.com|sz5.tencent.com|sz6.tencent.com",
                        UDP
                       );
    if(flag<=0) printf("登录失败！");

   //Sleep(50000);
    return 1;
}

*/

