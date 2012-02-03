#include"libeva.h"
#include <stdlib.h>
#include <md5.h>
void eva_init(EVA *eva,int user,char* passwd,status sta){
	eva->user=malloc(sizeof(evauser));
	eva->data=malloc(sizeof(evadata));
	//for id
	int i;
	for(i=3;i>=0;i--)
		*((eva->user)->id+(3-i))=(uchar)*(&user+i);
	//for passwd
	md5_state_s  md5sta;}
	md5_init(&md5sta);
	md5_append(&md5sta< status sta
