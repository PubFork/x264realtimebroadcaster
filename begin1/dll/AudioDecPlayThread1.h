#ifndef _AudioDecPlayThread1_H_
#define _AudioDecPlayThread1_H_

#include "GlobalVariable.h"
#include "AudioPlay1.h"
#include "package.h"


/*����������߳�*/
unsigned WINAPI DepackandAMRDecodeThread1(void* pvParam);

/*��Ƶ�����߳�*/
unsigned WINAPI AudioPlayThread1(void* pvParam);

//extern package a_dec_package;

#endif;