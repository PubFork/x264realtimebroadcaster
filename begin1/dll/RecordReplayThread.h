#ifndef _RecordReplayThread_H_
#define _RecordReplayThread_H_

#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>		//音频处理API函数mmsystem.h中声明


// 音频存储、回放 //
unsigned WINAPI RecordWavFileThread(void* pvParam);

unsigned WINAPI AudioReplayThread(void* pvParam);

// 视频存储、回放 //
unsigned WINAPI VideoRecordThread(void* pvParam);

unsigned WINAPI VideoReplayThread(void* pvParam);


#endif;