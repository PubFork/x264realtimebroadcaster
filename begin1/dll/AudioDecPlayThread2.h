#ifndef _AudioDecPlayThread2_H_
#define _AudioDecPlayThread2_H_

#include "GlobalVariable.h"
#include "AudioPlay2.h"
#include "package.h"


/*拆包、解码线程*/
unsigned WINAPI DepackandAMRDecodeThread2(void* pvParam);

/*音频播放线程*/
unsigned WINAPI AudioPlayThread2(void* pvParam);

//extern package a_dec_package;

#endif;