/*
2010-05-10 15:42  
���ӱ������壺 
static BOOL pRecvfromBufferQueueEmpty;
static BOOL pPCMForAudioOutQueueEmpty;
����  static int dwPCMForAudioOutQueueSize;

��
pRecvfromBufferQueue
pPCMForAudioOutQueue ���ӹؼ���ͬ����
*/
#include "stdafx.h"
#include "AudioDecPlayThread2.h"

//package a_dec_package;

unsigned WINAPI DepackandAMRDecodeThread2(void* pvParam)
{
	PCMRecSuccessFlag = FALSE;
	DWORD dwResult = 0;

	AMROutBuf		AMRDecBuf;
    AudioOutBuf		AudioDecBuf;

	//unsigned char AMR_Rec[38];
	//char RecvRTPFrameTemp[100];		//DTP�����õ���RTP֡

	//SocketInBuf RecvFrameTemp;//���յ�֡

	PBYTE pRecPCMFileBuffer = NULL;
	DWORD dwRecDataLength = 0;

	int lostNum = 0;  //������
	int Recvframe_count = 0;
	while(1)
	{
		static BOOL pRecvfromBufferQueue_2Empty;
		critical_section3_2.Lock();
		pRecvfromBufferQueue_2Empty = pRecvfromBufferQueue_2.empty();
		critical_section3_2.Unlock();

		if(!pRecvfromBufferQueue_2Empty)//���ն��зǿ���ȡ���ݽ��в�������빤��
		{	
			critical_section3_2.Lock();
			AMRDecBuf = pRecvfromBufferQueue_2.front();//�Ӷ���ȡ��һ֡
			critical_section3_2.Unlock();

			//a_dec_package.DTP_Depack(RecvRTPFrameTemp,RecvFrameTemp.buf);//DTP���
			//
			//a_dec_package.RTP_Depack(RecvRTPFrameTemp,AMR_Rec);//RTP������õ�AMR���ݴ����unsigned char analysis[32]��
			
			//AMRDecBuf.sequencenum = ((short)AMR_Rec[0]<<8)^((short)AMR_Rec[1]);
			//AMRDecBuf.timestamp   = ((long)AMR_Rec[2]<<24)^((long)AMR_Rec[3]<<16)^((long)AMR_Rec[4]<<8)^((long)AMR_Rec[5]);
			//for (int i=0;i<32;i++)
			//{
			//	AMRDecBuf.AMR_analysis[i]=AMR_Rec[i+6];
			//}
	
	        /* 11��19�����ڸĶ�������*/ 		
			m_AMR.AMR_Decode(AMRDecBuf.AMR_analysis, AudioDecBuf.speech);

			AudioDecBuf.sequencenum	= AMRDecBuf.sequencenum;

			AudioDecBuf.timestamp		= AMRDecBuf.timestamp;
			AudioDecBuf.timestamp		= AudioDecBuf.timestamp/160; 
			
// �ò��ִ�������¼����  [���뿪ʼ] //
			if(AudioRecordFlag == TRUE)
			{	
				printf("Receive Record Thread Start!");
				pRecPCMFileBuffer = (PBYTE)realloc(pRecPCMFileBuffer,dwRecDataLength+328); //�����������ڲ��Ų��ԣ�
				CopyMemory(pRecPCMFileBuffer+dwRecDataLength,&AudioDecBuf.sequencenum,328);
				dwRecDataLength+=328;
				if(dwRecDataLength>328*100)//���ÿ100֡����һ�Σ��������ù�����С
				                       //�������ɲ�ͬ������С���д�ļ�����Ƶ��������CPU��Դ
									   //������ֵʵ�����
				{
					PCMRecSuccessFlag=TRUE;
					TempLenR=dwRecDataLength;
					pTempPCMR=new BYTE[TempLenR];
					CopyMemory(pTempPCMR,pRecPCMFileBuffer,dwRecDataLength);
					EventPCMR.SetEvent();
					pRecPCMFileBuffer=new BYTE[0];
					dwRecDataLength=0;
				}

			}  

// �ò��ִ�������¼����  [�������] //

			static int dwPCMForAudioOutQueueSize;//
			critical_section4_2.Lock();
			dwPCMForAudioOutQueueSize=pPCMForAudioOutQueue_2.size();
			critical_section4_2.Unlock();

			if(dwPCMForAudioOutQueueSize<50)     //������г���Ϊ50֡������50֡������ݶ�ʧ
			{
				critical_section4_2.Lock();
				pPCMForAudioOutQueue_2.push(AudioDecBuf);       //������ϵ�PCM�����������
				critical_section4_2.Unlock();
			}
			else
			{
				critical_section4_2.Lock();
				for(int queueCount=0;queueCount<20;queueCount++)  //ɾ����֡����Ӱ�����������̶ȣ�����ʵ�ʲ������ѡ��
				{
					pPCMForAudioOutQueue_2.pop();
					lostNum++;
				}
				critical_section4_2.Unlock();
			}
#ifdef PRINTF_RECEIVE
			cout<<"lostNum="<<lostNum;
#endif
			EventPCMReceived.SetEvent();

			//printf("PCM���д�С%d\n",pPCMForAudioOutQueue.size());
			critical_section3_2.Lock();
			pRecvfromBufferQueue_2.pop();			//�����Ѿ���������봦���֡
			critical_section3_2.Unlock();
			Recvframe_count++;							//����֡��ͳ��

		}
		Sleep(1);//20����һ֡���ݣ��ȴ�10����ִ��һ�γ��򣬷�ֹCPUռ���ʴﵽ100%
	}

	printf("Thread OVER : DepackandAMRDecodeThread\n");

	return (dwResult);
}

unsigned WINAPI AudioPlayThread2(void* pvParam)
{
	//SynchronousFlag = TRUE;
//	if(pPCMForAudioOutQueue.size()>1)   // �����յ������ݴ���2֡ʱ�������������
//	{
	WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);
//	EventPCMReceived.ResetEvent();
//	WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);
	InitAudioOut2();		//��Ƶ����
//WavFileOutput();
//	break;
//	}
	
//	Sleep(100);
//	}

	printf("Thread OVER : AudioPlayThread\n");

	return 0;
}