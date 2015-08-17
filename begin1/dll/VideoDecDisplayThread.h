#ifndef _VideoDecDisplayThread_H
#define _VideoDecDisplayThread_H



#include "GlobalVariable.h"

#include "interf_h264.h"
#include "package.h"
#include "YUVdisplay.h"


unsigned WINAPI Depackand264DecodeThread(void* pvParam);


unsigned WINAPI VideoPlayThread(void* pvParam);


extern package v_dec_package;


#endif