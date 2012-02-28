#include <libeva.h>
#include <stdio.h>
/*
debug
*/
//测试QQ号
//帐号       - 密码
//2386953608 - 0123456789
//2421723768 - 0123456789
//847708268  - testtest
//1348186323 -012345678
int main(int agrc,char *argv[]){
	char qq[12];
	char passwd[16];
	int i=0;
	char in;
	printf("welcome to %s\n",eva_get_version());
	printf("QQ:");
	scanf("%s",qq);
	printf("Passwd:");
	while(1){
		in=getch();
		if(in=='\n'||in=='\r'||i==16) break;
		*(passwd+i)=in;
		fputc('*',stdout);
		i++;
	}
	*(passwd+i)='\0';

	EVA eva;    //申明一个EVA变量
	eva_init(&eva);  //EVA初使化
	int flag= eva_login(&eva,
              qq,passwd,EVA_ONLINE,
             "112.90.86.196|183.60.16.154|sz2.tencent.com|sz3.tencent.com|sz4.tencent.com|sz5.tencent.com|sz6.tencent.com",
	      UDP
             );
	if(flag==0) printf("net erro\n");
	if(flag==PASSWD_ERRO)printf("passwd erro");
	if(flag==ID_ERRO)printf("ID erro");
	if(flag==NO_ERRO)printf("go to jihuo");
	return 1;
}
