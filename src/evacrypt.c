#include "evacrypt.h"

#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

unsigned char randnum(void)  	  //这里是随机数生成
{
    return 0xad;   //这里是固定好呢还是随机好呢？
}
void entea(unsigned long *v, unsigned long *k, unsigned long *w)
{
    unsigned long y = ntohl(v[0]), z = ntohl(v[1]), a = ntohl(k[0]), b = ntohl(k[1]), c = ntohl(k[2]), d = ntohl(k[3]), n =16, sum = 0, delta = 0x9E3779B9;
    while (n-- > 0)
    {
        sum += delta;
        y += ((z << 4) + a)^(z + sum)^((z >> 5) + b);
        z += ((y << 4) + c)^(y + sum)^((y >> 5) + d);
    }			// while
    w[0] = htonl(y);
    w[1] = htonl(z);
}
void detea(unsigned long *v, unsigned long *k, unsigned long *w)
{
    unsigned long y = ntohl(v[0]), z = ntohl(v[1]), a = ntohl(k[0]), b =ntohl(k[1]), c = ntohl(k[2]), d = ntohl(k[3]), n = 0x10, sum = 0xE3779B90, delta = 0x9E3779B9;

    /* sum = delta<<5, in general sum = delta * n */
    while (n-- > 0)
    {
        z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
        y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
        sum -= delta;
    }

    w[0] = htonl(y);
    w[1] = htonl(z);
}

void qqencrypt( unsigned char* instr, int instrlen, unsigned char* key,
			unsigned char*  outstr, int* outstrlen_ptr)
{

    unsigned char  header;   //头字节
    //int header_flag=1;//头字节标记

    unsigned char rand=randnum();   //先获取随机填充数
    int time=0;   //在运算中计算次数用的
    int i=0;     //万能的I
    //unsigned char zero=0x00;
    unsigned char *plain=malloc  (8);         //加密后的8个数据
    unsigned char *pre_plain= malloc  (8);      //前加密后的8个数据
    unsigned char *crypted= malloc  (8);     //加密后的结果
    unsigned char *pre_crypted= malloc  (8);     //前次加密后的结果

   memset(pre_crypted,0,8);
   memset(pre_plain,0,8);


   int n=(instrlen+10)%8;                               //这里的n是用于计算机填充的个数
   unsigned char in[1024];
   memset(in,0,1024);


    if(n) n=8-n;  //此处为填充个数，取值范围为0至8


    header=(rand&(0xf8))|n;

    memcpy(in,&header,1);  //把头字节也填充进去

    for(time=0;time<n+2;time++)
    	memcpy(in+1+time,(unsigned char*)&rand,1);   //填充N＋2个随机数到instr头部

    memcpy(in+n+3,instr,instrlen);

   //最后还有7个0，因为我前面已经memset过了

   //此时的instr才是真正的被加密数据

    i=(n+2+8+instrlen)/8;//现在，我们的万能i记录了加密单位
    *outstrlen_ptr=(n+2+8+instrlen);



    time=0;   //time也用上了，懒得现声明新的变量
    while(i--)    //将加密数据读入plain
    {
        for(n=0; n<8; n++)
            *(plain+n)=*(in+8*time+n);
        for(n=0; n<8; n++)
        {
            *(plain+n)^=*(pre_crypted+n);
        }
        entea((unsigned long *)plain, (unsigned long *)key, (unsigned long  *)crypted);   //16轮的TEA

        for(n=0; n<8; n++)
        {
            *(crypted+n)^=*(pre_plain+n);
        }
//为下一次运算做准备
        for(n=0; n<8; n++)
        {
            *(pre_plain+n)=*(plain+n);
        }
        for(n=0; n<8; n++)
        {
            *(pre_crypted+n)=*(crypted+n);
        }
        memcpy (outstr+time*8,crypted, 8);
        time++;
    }
//像借钱一样，有借有还
    free  (pre_crypted);
    free  (crypted);
    free  (pre_plain);
    free  (plain);
}
int qqdecrypt( unsigned char* instr, int instrlen, unsigned char* key,
			unsigned char*  outstr, int* outstrlen_ptr)
{
    int len=instrlen;
    unsigned char out[1024];
    if ((len% 8) || (len < 16)) return 0;   //排除非tea加密的数据


    unsigned char *plain= malloc(8);         //加密的8个数据
    unsigned char *pre_plain= malloc(8);;     //前加密的8个数据
    unsigned char  *crypted= malloc(8);   //加密后的结果
    unsigned char  *crypted_buff= malloc(8);
    unsigned char  *pre_crypted= malloc(8);   //前次加密后的结果

    memset(pre_crypted,0,8);
    memset(pre_plain,0,8);

    int n;
    int i=len/8;
    int time=0;
    do
    {
        for(n=0; n<8; n++)  //将加密数据读入plain
        {
            *(plain+n)=*(instr+8*time+n);
        }
        for(n=0; n<8; n++)
        {
            *(crypted_buff+n)=*(plain+n);
	    *(plain+n)^=*(pre_plain+n);
        }
        detea((unsigned long*)plain, (unsigned long*)key, (unsigned long*)crypted);   //16轮的TEA
        for(n=0; n<8; n++)
        {
            *(crypted+n)^=*(pre_crypted+n);
        }
        //为下一次运算做准备
        for(n=0; n<8; n++)
        {
            *(pre_plain+n)=(*(pre_crypted+n))^(*(crypted+n));
        }
        for(n=0; n<8; n++)
        {
            *(pre_crypted+n)=*(crypted_buff+n);
        }
        memcpy (out+time*8,crypted, 8);
        time++;
    }
    while(--i);
//到这里还没有结束





    n=(int)out[0];
    n=n&7;
    n=n+2+1;

    for(i=1; i<8; i++)
    {
        if(*(out+instrlen-i)!=0x00)

        return 0;
    }

    memcpy(outstr,out+n,instrlen-n-7);
     *outstrlen_ptr=instrlen-n-7;


//像借钱一样，有借有还
    free  (pre_crypted);
    free  (crypted);
    free  (pre_plain);
    free  (plain);
    free  (crypted_buff);
    return 1;
}

/*debug
void main(int argc,char* argv[])
{

unsigned char keyn[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

unsigned char  instrn[4]={0x14,0x20,0x30,0x40};

unsigned char outstr[1024];
int outstrlen;
qqencrypt(  instrn, 4, keyn,
			outstr, &outstrlen);
unsigned char	outstr1[1024];
int outstr1len;
qqdecrypt(outstr,outstrlen,keyn,outstr1,&outstr1len);
printf("len:%d\n",outstr1len);
int i;
for( i=0;i<outstr1len;i++)
    printf("%X ",*(outstr1+i));

}
*/
