#ifndef _AudioPlay2_H_
#define _AudioPlay2_H_


#define MAX_BUFFER_SIZE_OUT 160
#define MAX_BUFFER_NUM_OUT 5
#include "GlobalVariable.h"
#include <afx.h>
#include <windows.h>
#include <mmsystem.h>  //������Ƶ����API����ȫ����mmsystem.h������������ʵ����wimmm.lib����
#pragma comment(lib,"winmm.lib")


static WAVEFORMATEX m_soundFormat_out2;//���������Ƶ��ʽ
static HWAVEOUT m_hWaveOut2;
static WAVEHDR m_pWaveHdrOut2[MAX_BUFFER_NUM_OUT];
static PBYTE pBufferOut2[MAX_BUFFER_NUM_OUT];  //�趨MAX_BUFFER_NUM_OUT�����ݻ���

static UINT BufferOutNo2; //BufferOut���
static BOOL bDevAudioOutOpen2;//�����豸�Ƿ�򿪱�־λ;

/*11.16�Ķ���������һ�����ڴ洢sq��ts������  ����ʼ��*/

static SQTS sqts2[MAX_BUFFER_NUM_OUT];

/*11.16�Ķ���������һ�����ڴ洢sq��ts������  ��������*/

static AudioOutBuf AudioOutBuf_2;
static short speechtmp2[MAX_BUFFER_SIZE_OUT];
static short speechBuf2[MAX_BUFFER_NUM_OUT][MAX_BUFFER_SIZE_OUT];

static int testcount12=0;

void CALLBACK WaveOutProc2(HWAVEOUT m_hWaveOut2, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
void WINAPI InitAudioOut2();
bool WINAPI AudioStop2();

#endif