#ifndef _AudioDecPlayThread1_H_
#define _AudioDecPlayThread1_H_

#include "GlobalVariable.h"
#include "AudioPlay1.h"
#include "package.h"


/*拆包、解码线程*/
unsigned WINAPI DepackandAMRDecodeThread1(void* pvParam);

/*音频播放线程*/
unsigned WINAPI AudioPlayThread1(void* pvParam);

//extern package a_dec_package;

#endif;