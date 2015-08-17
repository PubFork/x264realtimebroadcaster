
#ifndef _VideoSampleEncThread_H_
#define _VideoSampleEncThread_H_


#include "GlobalVariable.h"
#include "VideoSample.h"
#include "interf_x264.h"
//#include "package.h"

//unsigned WINAPI CameraInit();
//
//unsigned WINAPI CameraSmapleOpen();
//
//unsigned WINAPI CameraSampleClose();

unsigned WINAPI VideoSampleThread(void* pvParam); 


unsigned WINAPI H264EncodeAndPackageThread(void* pvParam);


//extern package v_enc_package;


#endif;