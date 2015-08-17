#ifndef _AudioSample_H_
#define _AudioSample_H_

#define MAX_BUFFER_SIZE 320
#define MAX_BUFFER_NUM 8


#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>		//��Ƶ����API����(WaveIn��WaveOut)��mmsystem.h������������ʵ����wimmm.lib����
#pragma comment(lib,"winmm.lib")

extern  PBYTE pFileSaveBuffer;
extern  DWORD dwDataLength;

static HWAVEIN m_hWaveIn;
static WAVEFORMATEX m_soundFormat;
static WAVEHDR m_pWaveHdrIn[MAX_BUFFER_NUM];
static PBYTE pBufferIn[MAX_BUFFER_NUM];  //�趨MAX_BUFFER_NUM�����ݻ���ָ��
static UINT BufferInNo;				//BufferIn���

void CALLBACK WaveInProc(HWAVEIN m_hWaveIn, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
void __stdcall InitAudio();
void __stdcall Record();
bool __stdcall StopRecord();

#endif;














/*CALLBACK�Ǳ�׼C�ķ�ʽ��C++�ඨ����鷳����ʱ�����޸ġ���ϸ�����μ�http://www.newty.de/fpt/callback.html */
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
	PBYTE pBufferIn[MAX_BUFFER_NUM];  //�趨MAX_BUFFER_NUM�����ݻ���ָ��
	UINT BufferInNo;				//BufferIn���
	PBYTE pFrameData;				//֡����ָ��
public:
	//C++�ص�������Ϊ���Ա����ʹ��static����
};
*/