#ifndef _RecordReplayThread_H_
#define _RecordReplayThread_H_

#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>		//��Ƶ����API����mmsystem.h������


// ��Ƶ�洢���ط� //
unsigned WINAPI RecordWavFileThread(void* pvParam);

unsigned WINAPI AudioReplayThread(void* pvParam);

// ��Ƶ�洢���ط� //
unsigned WINAPI VideoRecordThread(void* pvParam);

unsigned WINAPI VideoReplayThread(void* pvParam);


#endif;