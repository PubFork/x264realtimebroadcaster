#ifndef _AudioDecPlayThread2_H_
#define _AudioDecPlayThread2_H_

#include "GlobalVariable.h"
#include "AudioPlay2.h"
#include "package.h"


/*����������߳�*/
unsigned WINAPI DepackandAMRDecodeThread2(void* pvParam);

/*��Ƶ�����߳�*/
unsigned WINAPI AudioPlayThread2(void* pvParam);

//extern package a_dec_package;

#endif;