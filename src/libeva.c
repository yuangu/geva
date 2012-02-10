#include"libeva.h"
#include "socket.h"
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
    memcpy((eva->user)->id,&version,2);
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

    md5_init( &mst );
	md5_append( &mst, (md5_byte_t*)((eva->user)->passwd_md5_one),20);
	md5_finish( &mst, (md5_byte_t*)((eva->user)->passwd_md5_two) );

    //for status
    ((eva->user)->sta)=sta;
}
int eva_login(EVA* eva){

    LONDATA* logindata=eva_login_init();
    eva_login_touch(eva,logindata);
    return 1;
}
/*
debug
*/
int main(){
EVA eva;
eva_init(&eva,822705688,"123456",1);
eva.net=eva_net_init(UDP,"0.0.0.0",4000,
                      "219.133.49.171", 8000);

eva_login(&eva);
return 1;
}
