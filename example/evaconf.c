#include "evaconf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* getline(char *in,char* out,int lin){
	int num=0;
	char *swap,*outwp;
	outwp=out;
	swap=in;
	while(1){	
		if(*swap=='\n'){ num++;
		}else{
			if(num==(lin-1)) {*outwp=*swap; outwp++;}
		}
		if(num==lin) {*outwp='\0';break;}
		swap++;
		if(*swap=='\0') break;
	}
	return out;
}
void removespace(char *s)
{
	int i,j;
	i=0;
	while(s[i]!='\0'){
	if(s[i]==' '){  //这里是==，不是=。两个运算符是不同的。
		j=i;
		while(s[j+1]!='\0')
			{s[j]=s[j+1];
			j++;
			}
		s[j]='\0';
		}
	i++;
	}
}
int getlinenum(char *in){
	int num=0;	
	char *swap;
	swap=in;
	while(1){		
		if(*swap=='\n') num++;
		swap++;
		if(*swap=='\0') break;
	}
	if(num>0) num++;
	return num;
}
char* eva_conf_init(){
	FILE* file;
	char* conf;
	file=fopen("eva.conf","a+");
	if(file==NULL){printf("conffile can't load");return NULL;}
	conf=malloc(1024);
	memset(conf,0,1024);
	fread(conf,1,1024,file);
	return conf;

}
char* eva_conf_get(const char* conf,const char* name,char *value){
	int i;
	char *linearray=malloc(1000);	
	int linenum=getlinenum((char*)conf);
	for(i=1;i<=linenum;i++){
		memset(linearray,0,1000);
		getline((char*)conf,linearray,i);
		if(!strcmp(strstr(linearray,name),linearray)&&(*(linearray+strlen(name))=='='))
			memcpy(value,(linearray+strlen(name)+1),strlen((linearray+strlen(name)+1)));	

	}

	return value;
}
//debug
int main(){

	char *out=eva_conf_init();
	char* value=malloc(100);
	memset(value,0,100);
	eva_conf_get(out,"test",value);
	printf("%s",value);
	return 1;

}
