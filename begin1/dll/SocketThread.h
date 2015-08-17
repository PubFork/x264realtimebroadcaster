#define WM_TERM_THREAD 1023
#ifndef _SocketThread_H
#define _SocketThread_H

#include "GlobalVariable.h"

#include "package.h"


// Audio UDP Socket接收线程 //
unsigned WINAPI AudioUDPSocketReceiveThread(void* pvParam);

// Audio UDP Socket发送线程 //
unsigned WINAPI AudioUDPSocketSendThread(void* pvParam);

// Video UDP Socket接收线程 //
unsigned WINAPI VideoUDPSocketReceiveThread(void* pvParam);

// Video UDP Socket发送线程 //
unsigned WINAPI VideoUDPSocketSendThread(void* pvParam);

extern package a_enc_package;
extern package a_dec_package;

#endif