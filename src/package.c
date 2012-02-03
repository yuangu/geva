#include "package.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//init a evabyte
evabyte* initevabyte(evabyte* aevabyte,int maxlen){
	//evabyte aevabyte;
	aevabyte->data=(uchar*)malloc(maxlen);
	aevabyte->len=0;
	return aevabyte;
}
void freeevabyte(evabyte *aevabyte){
	free(aevabyte->data);
}
//连接两个字节，返回inevabyte的指针
evabyte* putevabyte(evabyte* inevabyte,const uchar* str,int len){
	int  i;
	for(i=0;i<len;i++)
	{	
		*((inevabyte->data)+(inevabyte->len))=str[i];
		inevabyte->len++;
	}
	return inevabyte;
	
}
evabyte* delevabyte(evabyte* inevabyte,int frist,int last){
	if((inevabyte->len)==last) inevabyte->len=frist;
       	else
	{	int f=frist,l=last,len=inevabyte->len;
		inevabyte->len=f;
		while(1){
			inevabyte->data[f++]=inevabyte->data[l++];
			inevabyte->len++;
			if(l==len) break;
		}

	}	
	return inevabyte;
	
}
uchar* getucharfromevabyte(evabyte* inevabyte,uchar* out,int first,int last){
	int i,f=first,l=last;
	if(f==l&&f==0) l=inevabyte->len;
	for(i=0;i<(l-f);i++)
		out[i]=*(inevabyte->data+f+i);

	return out;
	
}


/*
 //debug
int main()
{
	evabyte a;

	a.data=(uchar*)malloc(10);
	memset(a.data,0,sizeof(10));
	
	a.len=0;
	
	int b=822705688;
	putevabyte(&a,(char*)&b,4);
	uchar out;
	 getucharfromevabyte(&a,&out,1,2);
	
	//printf("%d\n",a.len);
//	int i;
//	for(i=0;i<2;i++)
	printf("%X\n",out);
	
	return 1;


}
*/
