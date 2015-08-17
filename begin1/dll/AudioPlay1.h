#ifndef _AudioPlay1_H_
#define _AudioPlay1_H_


#define MAX_BUFFER_SIZE_OUT 160
#define MAX_BUFFER_NUM_OUT 5
#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>  //������Ƶ����API����ȫ����mmsystem.h������������ʵ����wimmm.lib����
#pragma comment(lib,"winmm.lib")


static WAVEFORMATEX m_soundFormat_out1;//���������Ƶ��ʽ
static HWAVEOUT m_hWaveOut1;
static WAVEHDR m_pWaveHdrOut1[MAX_BUFFER_NUM_OUT];
static PBYTE pBufferOut1[MAX_BUFFER_NUM_OUT];  //�趨MAX_BUFFER_NUM_OUT�����ݻ���

static UINT BufferOutNo1; //BufferOut���
static BOOL bDevAudioOutOpen1;//�����豸�Ƿ�򿪱�־λ;

/*11.16�Ķ���������һ�����ڴ洢sq��ts������  ����ʼ��*/

static SQTS sqts1[MAX_BUFFER_NUM_OUT];

/*11.16�Ķ���������һ�����ڴ洢sq��ts������  ��������*/

static AudioOutBuf AudioOutBuf_1;
static short speechtmp1[MAX_BUFFER_SIZE_OUT];
static short speechBuf1[MAX_BUFFER_NUM_OUT][MAX_BUFFER_SIZE_OUT];

static int testcount11=0;

void CALLBACK WaveOutProc1(HWAVEOUT m_hWaveOut1, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
void WINAPI InitAudioOut1();
bool WINAPI AudioStop1();

#endif