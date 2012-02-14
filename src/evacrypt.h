#ifndef _CRYPT_H
#define _CRYPT_H

int qqdecrypt( unsigned char* instr, int instrlen, unsigned char* key,
			unsigned char*  outstr, int* outstrlen_ptr);
void qqencrypt( unsigned char* instr, int instrlen, unsigned char* key,
			unsigned char*  outstr, int* outstrlen_ptr);

#endif //_CRYPT_H
