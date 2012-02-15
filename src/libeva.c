#include"libeva.h"
#include "evasocket.h"
#include "login.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "md5.h"
void eva_init(EVA *eva,ulong user,char* passwd,status sta){
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
    int i;
    for(i=0;i<4;i++){
        *(md51passwithid+20+i)=*((eva->user)->id+i);
    }

    md5_init( &mst );
	md5_append( &mst, (md5_byte_t*)md51passwithid,24);
	md5_finish( &mst, (md5_byte_t*)((eva->user)->passwd_md5_two) );

    //for status
    ((eva->user)->sta)=sta;
}
int eva_login(EVA* eva){

                LONDATA* logindata=eva_login_init();
                int flag;


                flag=eva_login_touch_send(eva,logindata);
                  printf("%d",flag);
                if(flag<=0)return flag;
                flag=eva_login_touch_recv(eva,logindata);
                if(flag<=0) return flag; //0x91
                flag=eva_login_request_send(eva,logindata);
                if(flag<=0) return flag;
                flag=eva_login_request_recv(eva,logindata);
                if(flag<=0) return flag;   //0xAB
                flag=eva_login_verify_send(eva,logindata);
                if(flag<=0)return flag;
                flag=eva_login_verify_recv(eva,logindata);
                if(flag<=0) return flag;  //0xdd
                flag=eva_login_E5_send(eva,logindata);
                if(flag<=0) return flag;

                flag=eva_login_E5_recv(eva,logindata);
                if(flag<=0) return flag; //E5
                flag=eva_login_E3_send(eva,logindata);
                if(flag<=0) return flag;

                flag=eva_login_E3_recv(eva,logindata);
                if(flag<=0)return flag;//E3
                flag=eva_login_30_send(eva,logindata);
                if(flag<=0) return flag;
                flag=eva_login_30_recv(eva,logindata);

                if(flag<=0) return flag;


    printf("%d",flag);
    free(logindata);
    if(flag<=0) return flag;
    else
    return 1;
}
/*
debug
*/
//²âÊÔQQºÅ
//ÕÊºÅ       - ÃÜÂë
//2386953608 - 0123456789
//2421723768 - 0123456789
//847708268  - testtest
//1348186323 -012345678
int main(){
EVA eva;    //ÉêÃ÷Ò»¸öEVA±äÁ¿
eva_init(&eva,1348186323,"0123456789",EVA_ONLINE);  //EVA³õÊ¹»¯
//eva_init(&eva,847708268,"testtest",EVA_ONLINE);
eva.net=eva_net_init(UDP,"0.0.0.0",4000,
                      "183.60.48.32", 8000);  //ÍøÂç³õÊ¹»¯
if(eva.net==NULL)printf("³ö´íÁË£¬net³õÊ¹»¯£¡");
else{
int flag=eva_login(&eva);     //µÇÂ¼
printf("%d",flag);
if(flag<=0) printf("µÇÂ¼Ê§°Ü£¡");
}
return 1;
}

