#ifndef _LIB_EVA_H_
#define _LIB_EVA_H_
#include "evadef.h"
#include "socket.h"
typedef struct{
	evanet* net;
	evauser* user;
	evadata* data;
} EVA;

#endif
