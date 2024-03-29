#ifndef _AudioPlay2_H_
#define _AudioPlay2_H_


#define MAX_BUFFER_SIZE_OUT 160
#define MAX_BUFFER_NUM_OUT 5
#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>  //波形音频处理API函数全部在mmsystem.h中声明，函数实现在wimmm.lib库中
#pragma comment(lib,"winmm.lib")


static WAVEFORMATEX m_soundFormat_out2;//输入输出音频格式
static HWAVEOUT m_hWaveOut2;
static WAVEHDR m_pWaveHdrOut2[MAX_BUFFER_NUM_OUT];
static PBYTE pBufferOut2[MAX_BUFFER_NUM_OUT];  //设定MAX_BUFFER_NUM_OUT个数据缓存

static UINT BufferOutNo2; //BufferOut编号
static BOOL bDevAudioOutOpen2;//播音设备是否打开标志位;

/*11.16改动：增加了一个用于存储sq、ts的数组  （开始）*/

static SQTS sqts2[MAX_BUFFER_NUM_OUT];

/*11.16改动：增加了一个用于存储sq、ts的数组  （结束）*/

static AudioOutBuf AudioOutBuf_2;
static short speechtmp2[MAX_BUFFER_SIZE_OUT];
static short speechBuf2[MAX_BUFFER_NUM_OUT][MAX_BUFFER_SIZE_OUT];

static int testcount12=0;

void CALLBACK WaveOutProc2(HWAVEOUT m_hWaveOut2, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
void WINAPI InitAudioOut2();
bool WINAPI AudioStop2();

#endif