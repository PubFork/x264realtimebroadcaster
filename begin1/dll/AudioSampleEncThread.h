#ifndef _AudioSampleEncThread_H_
#define _AudioSampleEncThread_H_

#include "GlobalVariable.h"
#include "AudioSample.h"
//#include "package.h"


/*音频采集线程*/
unsigned WINAPI AudioSampleThread(void* pvParam); 

/*编码、封包线程*/
unsigned WINAPI AMREncodeAndPackageThread(void* pvParam);

//extern package a_enc_package;

#endif;
