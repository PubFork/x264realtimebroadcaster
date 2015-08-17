#ifndef _AudioSample_H_
#define _AudioSample_H_

#define MAX_BUFFER_SIZE 320
#define MAX_BUFFER_NUM 8


#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>		//音频处理API函数(WaveIn、WaveOut)在mmsystem.h中声明，函数实现在wimmm.lib库中
#pragma comment(lib,"winmm.lib")

extern  PBYTE pFileSaveBuffer;
extern  DWORD dwDataLength;

static HWAVEIN m_hWaveIn;
static WAVEFORMATEX m_soundFormat;
static WAVEHDR m_pWaveHdrIn[MAX_BUFFER_NUM];
static PBYTE pBufferIn[MAX_BUFFER_NUM];  //设定MAX_BUFFER_NUM个数据缓存指针
static UINT BufferInNo;				//BufferIn编号

void CALLBACK WaveInProc(HWAVEIN m_hWaveIn, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
void __stdcall InitAudio();
void __stdcall Record();
bool __stdcall StopRecord();

#endif;














/*CALLBACK是标准C的方式，C++类定义较麻烦，有时间再修改。详细方法参见http://www.newty.de/fpt/callback.html */
/*
extern DWORD CALLBACK WaveInProc(HWAVEIN m_hWaveIn, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
class AudioSample
{
	friend DWORD CALLBACK WaveInProc(HWAVEIN m_hWaveIn, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
public:
	AudioSample(void);
	~AudioSample(void);
	void StartSample(void);
	void StopSample(void);

private:
	HWAVEIN m_hWaveIn;
	WAVEFORMATEX m_soundFormat;
	WAVEHDR m_pWaveHdrIn[MAX_BUFFER_NUM];
	PBYTE pBufferIn[MAX_BUFFER_NUM];  //设定MAX_BUFFER_NUM个数据缓存指针
	UINT BufferInNo;				//BufferIn编号
	PBYTE pFrameData;				//帧数据指针
public:
	//C++回调函数作为类成员必须使用static修饰
};
*/