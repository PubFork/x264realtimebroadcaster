#ifndef _AudioSampleEncThread_H_
#define _AudioSampleEncThread_H_

#include "GlobalVariable.h"
#include "AudioSample.h"
//#include "package.h"


/*��Ƶ�ɼ��߳�*/
unsigned WINAPI AudioSampleThread(void* pvParam); 

/*���롢����߳�*/
unsigned WINAPI AMREncodeAndPackageThread(void* pvParam);

//extern package a_enc_package;

#endif;
