#include "package.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "evacrypt.h"
//init a evabyte
evabyte* initevabyte(evabyte* aevabyte,int maxlen){
	//evabyte aevabyte;
	aevabyte->data=(uchar*)malloc(maxlen);
	aevabyte->len=0;
	memset(aevabyte->data,0,maxlen);
	return aevabyte;
}
void freeevabyte(evabyte *aevabyte){
	free(aevabyte->data);
	aevabyte->data=NULL;
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
	{	int f=frist,l=last;
		inevabyte->len=0;
		int i=0;
		while(1){
		    *(inevabyte->data+(i++))=*(inevabyte->data+(f++));
            inevabyte->len++;
            if(f==l)break;
		}

	}
	return inevabyte;

}
uchar* getucharfromevabyte(evabyte* inevabyte,uchar* out,int first,int last){
    int i;
    for(i=0;i<(last-first);i++)
		*(out+i)=*(((inevabyte->data)+first)+i);

	return out;

}
evabyte * encryptevabyte(const evabyte* inbyte,evabyte *outbyte,uchar* key){
    initevabyte(outbyte,MAX_LEN);
    qqencrypt( inbyte->data,inbyte->len,key,
			outbyte->data,&(outbyte->len));
    return outbyte;

}
void  seqadd(uchar* seq){
    if(*(seq+1)==0xFF){
        *(seq+1)=0;
        if(*seq==0xff)
            *(seq)=0;
        else *seq=*seq+1;
    }else{
        *(seq+1)=*(seq+1)+1;
    }
}

