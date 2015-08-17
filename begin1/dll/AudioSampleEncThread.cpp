/*
2010-3-29 21:06  
���ӱ������壺 
static BOOL pDataBufferQueueEmpty;
static BOOL pSocketSendQuequeEmpty;
�� pDataBufferQueque.empty()��FrameTemp=pDataBufferQueque.front();��pSocketSendQueque.empty()��SocketSendBuf=pSocketSendQueque.front();���ӹؼ���
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

unsigned WINAPI AMREncodeAndPackageThread(void* pvParam)//AMR�����߳�
{
	DWORD dwResult=0;

	short audio_speech[160];
	unsigned char AMR_data[32];

	//char m_RTPFrame[44];//RTP֡���ݴ������;
	//int  m_RTPFramelength;//RTP֡�ĳ���
	//char m_DTPFrame[53];//DTP֡���ݴ������;

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
				//���ܻ��ó���ҵ�!!!!
				//printf("%x\n",audio_speech[i]);//ǿ��ת��Ϊword���͵�ָ�룬PCMһ������Ϊ16bits���������ֽ�һ�����ݶ�ȡ
			}

			m_AMR.AMR_Encode(audio_speech, AMR_data);										//AMR����
			
			
		

			memcpy(AMREncBuf.AMR_analysis, AMR_data, 32);

		

			AMREncBuf.sequencenum ++;

			//m_RTPFramelength = a_enc_package.RTP_Pack(AMR_data, m_RTPFrame, frame_count);			//RTP���

			//m_SocketOutFrame.pFrame		= m_RTPFrame;
			//m_SocketOutFrame.Framelen	= m_RTPFramelength;

			//int m_DTPFramelength		= a_enc_package.DTP_Pack(m_RTPFrame,m_DTPFrame,m_RTPFramelength);
			//m_SocketOutFrame.pFrame		= m_DTPFrame;
			//m_SocketOutFrame.Framelen	= m_DTPFramelength;


			critical_section2.Lock();
			pSocketSendQueque.push(AMREncBuf);
			critical_section2.Unlock();


			//����
			critical_section1.Lock();//#####���´����Ϊ�ٽ���������ɼ��߳�ͬʱ�Զ��в�����lock()�����ٽ�����#####
			pDataBufferQueque.pop();
			critical_section1.Unlock();//#####ִ����Ϻ󣬽Ӵ��ٽ���������#####
			frame_count++;//����֡��ͳ��,RTP���õ�
		}
		//if(frame_count==1) break;//����һ֡
		//Sleep(1);//Sleep(10);//20����һ֡���ݣ��ȴ�10����ִ��һ�γ��򣬷�ֹCPUռ���ʴﵽ100%
	}

	printf("Thread OVER : AMREncodeAndPackageThread\n");

	return (dwResult);
}
