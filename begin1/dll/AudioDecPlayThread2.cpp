/*
2010-05-10 15:42  
增加变量定义： 
static BOOL pRecvfromBufferQueueEmpty;
static BOOL pPCMForAudioOutQueueEmpty;
增加  static int dwPCMForAudioOutQueueSize;

对
pRecvfromBufferQueue
pPCMForAudioOutQueue 增加关键区同步；
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
	//char RecvRTPFrameTemp[100];		//DTP拆包后得到的RTP帧

	//SocketInBuf RecvFrameTemp;//接收的帧

	PBYTE pRecPCMFileBuffer = NULL;
	DWORD dwRecDataLength = 0;

	int lostNum = 0;  //丢包数
	int Recvframe_count = 0;
	while(1)
	{
		static BOOL pRecvfromBufferQueue_2Empty;
		critical_section3_2.Lock();
		pRecvfromBufferQueue_2Empty = pRecvfromBufferQueue_2.empty();
		critical_section3_2.Unlock();

		if(!pRecvfromBufferQueue_2Empty)//接收队列非空则取数据进行拆包、解码工作
		{	
			critical_section3_2.Lock();
			AMRDecBuf = pRecvfromBufferQueue_2.front();//从队列取出一帧
			critical_section3_2.Unlock();

			//a_dec_package.DTP_Depack(RecvRTPFrameTemp,RecvFrameTemp.buf);//DTP拆包
			//
			//a_dec_package.RTP_Depack(RecvRTPFrameTemp,AMR_Rec);//RTP拆包，得到AMR数据存放在unsigned char analysis[32]中
			
			//AMRDecBuf.sequencenum = ((short)AMR_Rec[0]<<8)^((short)AMR_Rec[1]);
			//AMRDecBuf.timestamp   = ((long)AMR_Rec[2]<<24)^((long)AMR_Rec[3]<<16)^((long)AMR_Rec[4]<<8)^((long)AMR_Rec[5]);
			//for (int i=0;i<32;i++)
			//{
			//	AMRDecBuf.AMR_analysis[i]=AMR_Rec[i+6];
			//}
	
	        /* 11月19日王冠改动：解码*/ 		
			m_AMR.AMR_Decode(AMRDecBuf.AMR_analysis, AudioDecBuf.speech);

			AudioDecBuf.sequencenum	= AMRDecBuf.sequencenum;

			AudioDecBuf.timestamp		= AMRDecBuf.timestamp;
			AudioDecBuf.timestamp		= AudioDecBuf.timestamp/160; 
			
// 该部分代码用于录音。  [代码开始] //
			if(AudioRecordFlag == TRUE)
			{	
				printf("Receive Record Thread Start!");
				pRecPCMFileBuffer = (PBYTE)realloc(pRecPCMFileBuffer,dwRecDataLength+328); //保存数据用于播放测试；
				CopyMemory(pRecPCMFileBuffer+dwRecDataLength,&AudioDecBuf.sequencenum,328);
				dwRecDataLength+=328;
				if(dwRecDataLength>328*100)//大概每100帧保存一次，不能设置过大或过小
				                       //过大会造成不同步；过小造成写文件操作频繁，暂用CPU资源
									   //具体数值实测决定
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

// 该部分代码用于录音。  [代码结束] //

			static int dwPCMForAudioOutQueueSize;//
			critical_section4_2.Lock();
			dwPCMForAudioOutQueueSize=pPCMForAudioOutQueue_2.size();
			critical_section4_2.Unlock();

			if(dwPCMForAudioOutQueueSize<50)     //缓冲队列长度为50帧，超过50帧后的数据丢失
			{
				critical_section4_2.Lock();
				pPCMForAudioOutQueue_2.push(AudioDecBuf);       //解码完毕的PCM数据送入队列
				critical_section4_2.Unlock();
			}
			else
			{
				critical_section4_2.Lock();
				for(int queueCount=0;queueCount<20;queueCount++)  //删除的帧数，影响语音流畅程度，根据实际测试情况选择
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

			//printf("PCM队列大小%d\n",pPCMForAudioOutQueue.size());
			critical_section3_2.Lock();
			pRecvfromBufferQueue_2.pop();			//销毁已经拆包、解码处理的帧
			critical_section3_2.Unlock();
			Recvframe_count++;							//解码帧数统计

		}
		Sleep(1);//20毫秒一帧数据，等待10毫秒执行一次程序，防止CPU占用率达到100%
	}

	printf("Thread OVER : DepackandAMRDecodeThread\n");

	return (dwResult);
}

unsigned WINAPI AudioPlayThread2(void* pvParam)
{
	//SynchronousFlag = TRUE;
//	if(pPCMForAudioOutQueue.size()>1)   // 当接收到的数据大于2帧时才启动声卡输出
//	{
	WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);
//	EventPCMReceived.ResetEvent();
//	WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);
	InitAudioOut2();		//音频播放
//WavFileOutput();
//	break;
//	}
	
//	Sleep(100);
//	}

	printf("Thread OVER : AudioPlayThread\n");

	return 0;
}