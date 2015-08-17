#include "stdafx.h"
#include "AudioSample.h"

PBYTE pFileSaveBuffer;
DWORD dwDataLength;

void CALLBACK WaveInProc(HWAVEIN m_hWaveIn, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2)
{
	//printf("Audio Callback %x\n",uMsg);
	switch(uMsg) 
	{
		case WIM_OPEN:
			break;

		case WIM_CLOSE:
			break;

		case WIM_DATA:
			waveInUnprepareHeader(m_hWaveIn,&m_pWaveHdrIn[BufferInNo],sizeof(WAVEHDR));


		if(m_pWaveHdrIn[BufferInNo].dwBytesRecorded == MAX_BUFFER_SIZE)
		{
			critical_section1.Lock();//加锁
			pDataBufferQueque.push(pBufferIn[BufferInNo]);//保存帧数据的指针到队列
			critical_section1.Unlock();//解锁


/*录音处理程序段-开始-*/

/* // 1.0 源程序
if(bRecordThread==TRUE)
			{
				printf("Send Record Thread Start!");
				pFileSaveBuffer= (PBYTE)realloc (pFileSaveBuffer, dwDataLength+m_pWaveHdrIn[BufferInNo].dwBytesRecorded);
				CopyMemory(pFileSaveBuffer + dwDataLength, (PBYTE)pBufferIn[BufferInNo],m_pWaveHdrIn[BufferInNo].dwBytesRecorded);
				dwDataLength+=m_pWaveHdrIn[BufferInNo].dwBytesRecorded ;
				if(dwDataLength>320*100)//大概每100K保存一次
				{
#ifdef PRINTF_RECORD
					printf("send in\n");
#endif;
					PCMSendSuccessFlag=TRUE;  //成功采集到发送的100K数据
					TempLenS=dwDataLength;    //将采集到的数据转移用于文件保存
					pTempPCMS=new BYTE[TempLenS];
					CopyMemory(pTempPCMS,pFileSaveBuffer,dwDataLength);
					EventPCMS.SetEvent();     //通知写录音写文件线程可以开始语音合成。
					dwDataLength=0;
					pFileSaveBuffer=new BYTE[0]; //丢弃已写的发送PCM数据，释放内存;
#ifdef PRINTF_RECORD
					printf("send out\n");
#endif;
				}
			}

		}
*/
/*录音处理程序段-结束-*/
/* 11月22日王冠改动：  */ 
			
if(AudioRecordFlag == TRUE)
			{
				// printf("Send Record Thread Start!");
				//  m_pWaveHdrIn[BufferInNo].dwBytesRecorded 是定值 320。
				pFileSaveBuffer= (PBYTE)realloc (pFileSaveBuffer, dwDataLength+m_pWaveHdrIn[BufferInNo].dwBytesRecorded+8);
				unsigned char zero_a[8]={0};
				memcpy(pFileSaveBuffer + dwDataLength,zero_a ,8);
				CopyMemory(pFileSaveBuffer + dwDataLength + 8, (PBYTE)pBufferIn[BufferInNo],m_pWaveHdrIn[BufferInNo].dwBytesRecorded);




				dwDataLength+=328;

				if(dwDataLength>328*100)//大概每100K保存一次
				{
#ifdef PRINTF_RECORD
					printf("send in\n");
#endif;
					PCMSendSuccessFlag=TRUE;  //成功采集到发送的100K数据
					TempLenS=dwDataLength;    //将采集到的数据转移用于文件保存
					pTempPCMS=new BYTE[TempLenS];
					CopyMemory(pTempPCMS,pFileSaveBuffer,dwDataLength);
					EventPCMS.SetEvent();     //通知写录音写文件线程可以开始语音合成。
					dwDataLength=0;
					pFileSaveBuffer=new BYTE[0]; //丢弃已写的发送PCM数据，释放内存;
#ifdef PRINTF_RECORD
					printf("send out\n");
#endif;
				}
			}

		}


		//准备下一个语音采集缓冲区
		waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[BufferInNo],sizeof(WAVEHDR));
		waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[BufferInNo],sizeof(WAVEHDR));
		BufferInNo++;
		if(BufferInNo==MAX_BUFFER_NUM) 
		{
			BufferInNo=0;
		}
		break;
	};
}

void __stdcall InitAudio()
{
	dwDataLength=0;
	BufferInNo=0;
	PCMSendSuccessFlag=FALSE;

	m_soundFormat.wFormatTag=WAVE_FORMAT_PCM;
	m_soundFormat.nChannels=1;
	m_soundFormat.nSamplesPerSec=8000;
	m_soundFormat.nAvgBytesPerSec=16000;
	m_soundFormat.nBlockAlign=2;
	m_soundFormat.cbSize=0;
	m_soundFormat.wBitsPerSample=16;     //指定录音格式

	waveInOpen(&m_hWaveIn,WAVE_MAPPER, &m_soundFormat,(DWORD)WaveInProc,NULL,CALLBACK_FUNCTION); //打开录音设备
	for(int i=0;i<MAX_BUFFER_NUM;i++)
	{	
		pBufferIn[i]=(PBYTE)malloc(MAX_BUFFER_SIZE);
		m_pWaveHdrIn[i].lpData=(LPSTR)pBufferIn[i];
		m_pWaveHdrIn[i].dwBufferLength=MAX_BUFFER_SIZE;
		m_pWaveHdrIn[i].dwUser=0;
		m_pWaveHdrIn[i].dwBytesRecorded=0;
		m_pWaveHdrIn[i].dwFlags=0;
		waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[i],sizeof(WAVEHDR)); //准备内存块录音
		waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[i],sizeof(WAVEHDR)); //增加内存块
	}
}

void __stdcall Record()
{
	waveInStart(m_hWaveIn);//开始录音
}

bool __stdcall StopRecord()
{
	waveInStop(m_hWaveIn); //停止录音
	waveInReset(m_hWaveIn); //清空内存块
    return true;
}

