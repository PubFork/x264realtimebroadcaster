/*
2010-3-29 21:06  
增加变量定义： 
static BOOL pDataBufferQueueEmpty;
static BOOL pSocketSendQuequeEmpty;
对 pDataBufferQueque.empty()，FrameTemp=pDataBufferQueque.front();和pSocketSendQueque.empty()，SocketSendBuf=pSocketSendQueque.front();增加关键区
*/
#include "stdafx.h"
#include "AudioSampleEncThread.h"

//package a_enc_package;

unsigned WINAPI AudioSampleThread(void* pvParam)
{
	InitAudio();
	Record();
	//printf("Thread OVER : AudioSampleThread\n");
	return 0;
}

unsigned WINAPI AMREncodeAndPackageThread(void* pvParam)//AMR编码线程
{
	DWORD dwResult=0;

	short audio_speech[160];
	unsigned char AMR_data[32];

	//char m_RTPFrame[44];//RTP帧数据存放数组;
	//int  m_RTPFramelength;//RTP帧的长度
	//char m_DTPFrame[53];//DTP帧数据存放数组;

	AMROutBuf AMREncBuf;

	//SocketOutFrame m_SocketOutFrame;

	DWORD frame_count = 0;

	PBYTE FrameTemp;

	while(1)
	{
		//if (ThreadEnd == TRUE) break;

		static BOOL pDataBufferQueueEmpty;
		critical_section1.Lock();
		pDataBufferQueueEmpty = pDataBufferQueque.empty();
		critical_section1.Unlock();

		if(!pDataBufferQueueEmpty)
		{
			FrameTemp = NULL;
			critical_section1.Lock();
			FrameTemp = pDataBufferQueque.front();
			critical_section1.Unlock();
			for( int i=0; i<160; i++ )
			{
				audio_speech[i] = 0;
				audio_speech[i] = *((PWORD)FrameTemp+i);
				//可能会让程序挂掉!!!!
				//printf("%x\n",audio_speech[i]);//强制转换为word类型的指针，PCM一个采样为16bits，按两个字节一个数据读取
			}

			m_AMR.AMR_Encode(audio_speech, AMR_data);										//AMR编码
			
			
		

			memcpy(AMREncBuf.AMR_analysis, AMR_data, 32);

		

			AMREncBuf.sequencenum ++;

			//m_RTPFramelength = a_enc_package.RTP_Pack(AMR_data, m_RTPFrame, frame_count);			//RTP封包

			//m_SocketOutFrame.pFrame		= m_RTPFrame;
			//m_SocketOutFrame.Framelen	= m_RTPFramelength;

			//int m_DTPFramelength		= a_enc_package.DTP_Pack(m_RTPFrame,m_DTPFrame,m_RTPFramelength);
			//m_SocketOutFrame.pFrame		= m_DTPFrame;
			//m_SocketOutFrame.Framelen	= m_DTPFramelength;


			critical_section2.Lock();
			pSocketSendQueque.push(AMREncBuf);
			critical_section2.Unlock();


			//发送
			critical_section1.Lock();//#####以下代码段为临界区，避免采集线程同时对队列操作，lock()锁定临界区。#####
			pDataBufferQueque.pop();
			critical_section1.Unlock();//#####执行完毕后，接触临界区锁定。#####
			frame_count++;//编码帧数统计,RTP需用到
		}
		//if(frame_count==1) break;//编码一帧
		//Sleep(1);//Sleep(10);//20毫秒一帧数据，等待10毫秒执行一次程序，防止CPU占用率达到100%
	}

	printf("Thread OVER : AMREncodeAndPackageThread\n");

	return (dwResult);
}
