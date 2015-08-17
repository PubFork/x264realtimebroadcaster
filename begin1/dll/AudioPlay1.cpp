/*2010-05-10
增加： pPCMForAudioOutQueue的关键区同步
变量定义：
static BOOL pPCMForAudioOutQueueEmpty;
增加关键区同步 pPCMForAudioOutQueue
*/
#include "stdafx.h"
#include "AudioPlay1.h"

void CALLBACK WaveOutProc1(HWAVEOUT m_hWaveOut1, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2)
{
	switch(uMsg)
	{

		case WOM_OPEN: 
			bDevAudioOutOpen1=true;
#ifdef PRINTF_SEND
			cout<<"\nAudio Device is OPEN!"<<endl;
#endif
			break;
		case WOM_CLOSE:
			bDevAudioOutOpen1=false;
#ifdef PRINTF_RECEIVE
			cout<<"\nAudio Device is CLOSE!"<<endl;
#endif
			break;
		case WOM_DONE:
#ifdef PRINTF_RECEIVE
			cout<<"WOM_DATA_DONE"<<testcount11<<endl;
#endif
			testcount11++;
/*11.16改动：输出当前播放的那一帧PCM数据对应的sq和ts值，即在sqts数组的当前指针的下一个 （开始）*/

		    if(BufferOutNo1 == MAX_BUFFER_NUM_OUT-1)  //MAX_BUFFER_NUM_OUT=5
			{
				//cout<<"Sequence number "<<sqts[0].sq<<" - "<<"Timestamp number "<<sqts[0].ts<<"\n";
				SQTScontrol.sq = sqts1[0].sq;
				SQTScontrol.ts = sqts1[0].ts;
			}
			else
			{
				//cout<<"Sequence number "<<sqts[BufferOutNo+1].sq<<" - "<<"Timestamp number "<<sqts[BufferOutNo+1].ts<<"\n";
				SQTScontrol.sq = sqts1[BufferOutNo1+1].sq;
				SQTScontrol.ts = sqts1[BufferOutNo1+1].ts;
			}


/*11.16改动：输出当前播放的那一帧PCM数据对应的sq和ts值，即在sqts数组的当前指针的下一个  （结束）*/

		/*方法二：开始*/
			PWAVEHDR pWaveHeader=((PWAVEHDR)dwParam1);//Step1: 获取已经播放完毕的WAVEHDR
//			cout<<pWaveHeader<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n"; //输出缓冲区的地址，调试用
			waveOutUnprepareHeader(m_hWaveOut1,pWaveHeader,sizeof(WAVEHDR)); //Step2: 释放声卡设备缓存
//			cout<<pWaveHeader<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n";
	//		DONE_Count++;
	//		if(DONE_Count==MAX_BUFFER_NUM)
	//		{
  
//			if(pPCMForAudioOutQueue.empty())
//			{
			//printf("event2-");
		   // WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);//无限等待PCM数据产生事件，才开始播放.
			//printf("event\n");

//				EventPCMReceived.ResetEvent();

//			}
//
			//if(!pPCMForAudioOutQueue.empty())
//			AudioOutBuf2=pPCMForAudioOutQueue.front();// 从队列中取出一个数据包

			//pBufferOut[BufferOutNo]=(PBYTE)AudioOutBuf2.speech;
//			pBufferOut[BufferOutNo]=(PBYTE)speechtmp;
#ifdef PRINTF_RECEIVE
cout<<"pPCMForAudioOutQueue.size="<<pPCMForAudioOutQueue.size();
#endif
		static BOOL pPCMForAudioOutQueue_1Empty;
		critical_section4_1.Lock();
		pPCMForAudioOutQueue_1Empty=pPCMForAudioOutQueue_1.empty();
		critical_section4_1.Unlock();
		if(!pPCMForAudioOutQueue_1Empty)
		{
			
			critical_section4_1.Lock();
			AudioOutBuf_1=pPCMForAudioOutQueue_1.front();// 从队列中取出一个数据包
			pPCMForAudioOutQueue_1.pop();
			critical_section4_1.Unlock();
	
			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf1[BufferOutNo1][counttmp2]=AudioOutBuf_1.speech[counttmp2];          //将解包出的PCM数据装入结构体AudioDecBuf
			}
		}
		else
		{
			for(int counttmp3=0;counttmp3<160;counttmp3++)
			{
				speechBuf1[BufferOutNo1][counttmp3]=0;          //静音，填充0
			}
		}

	/*11.16改动：新抓入的一阵AudioOutBuf数据在被拆解时顺便把其中的序列号、时间戳信息提取出来赋给sqts数组记录  （结束）*/
	
		sqts1[BufferOutNo1].sq = AudioOutBuf_1.sequencenum;
		sqts1[BufferOutNo1].ts = AudioOutBuf_1.timestamp;


/* 11月24日 改动：

	if(!pPCMForAudioOutQueueEmpty)
		{
			
			critical_section4.Lock();
			AudioOutBuf2=pPCMForAudioOutQueue.front();// 从队列中取出一个数据包
			pPCMForAudioOutQueue.pop();
			critical_section4.Unlock();
	
			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf[BufferOutNo][counttmp2]=AudioOutBuf2.speech[counttmp2];          //将解包出的PCM数据装入结构体AudioDecBuf
				
				sqts[BufferOutNo].sq=AudioOutBuf2.sequencenum;
				sqts[BufferOutNo].ts=AudioOutBuf2.timestamp;

			}
		}
		else
		{
			for(int counttmp3=0;counttmp3<160;counttmp3++)
			{
				speechBuf[BufferOutNo][counttmp3]=0;          //静音，填充0
				if(BufferOutNo == 0)
					{
					 sqts[BufferOutNo].sq=sqts[4].sq+1;
				     sqts[BufferOutNo].ts=sqts[4].ts+160;

					}
				else
					{
					 sqts[BufferOutNo].sq=sqts[BufferOutNo-1].sq+1;
				     sqts[BufferOutNo].ts=sqts[BufferOutNo-1].ts+160;

					}



			}
		}



*/

			//pBufferOut[BufferOutNo]=(PBYTE)speechtmp;


			(*pWaveHeader).lpData =(LPSTR)speechBuf1[BufferOutNo1];
			//m_pWaveHdrOut[BufferOutNo].lpData =(LPSTR)pBufferOut[BufferOutNo];
//			pPCMForAudioOutQueue.pop();
			(*pWaveHeader).dwBufferLength = 2*MAX_BUFFER_SIZE_OUT;
			(*pWaveHeader).dwFlags=0;
//cout<<&m_pWaveHdrOut[BufferOutNo]<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n";

/*			m_pWaveHdrOut[BufferOutNo].lpData =(LPSTR)speechBuf[BufferOutNo];
			//m_pWaveHdrOut[BufferOutNo].lpData =(LPSTR)pBufferOut[BufferOutNo];
//			pPCMForAudioOutQueue.pop();
			m_pWaveHdrOut[BufferOutNo].dwBufferLength =MAX_BUFFER_SIZE;
			m_pWaveHdrOut[BufferOutNo].dwFlags=0;*/
		//	m_pWaveHdrOut[BufferOutNo].dwBytesRecorded=0;


			waveOutPrepareHeader( m_hWaveOut1, &m_pWaveHdrOut1[BufferOutNo1], sizeof(WAVEHDR) );
			waveOutWrite( m_hWaveOut1, &m_pWaveHdrOut1[BufferOutNo1], sizeof(WAVEHDR) );
			
			BufferOutNo1++;
			if(BufferOutNo1==MAX_BUFFER_NUM_OUT) {BufferOutNo1=0;}


	//		}
			
			break;
		/*方法二：结束*/


	}
}
void WINAPI InitAudioOut1()
{
    bDevAudioOutOpen1=false;
	BufferOutNo1=0;

	m_soundFormat_out1.wFormatTag=WAVE_FORMAT_PCM;
	m_soundFormat_out1.nChannels=1;
	m_soundFormat_out1.nSamplesPerSec=8000;
	m_soundFormat_out1.nAvgBytesPerSec=16000;
	m_soundFormat_out1.nBlockAlign=2;
	m_soundFormat_out1.cbSize=0;
	m_soundFormat_out1.wBitsPerSample=16;     //指定播音格式
	
	waveOutOpen(&m_hWaveOut1,WAVE_MAPPER,&m_soundFormat_out1,(DWORD)WaveOutProc1,NULL,CALLBACK_FUNCTION);//打开播音设备
	
	waveOutPause(m_hWaveOut1);//先暂停输出，将所有缓冲区填充满后再输出

	waveOutSetVolume(m_hWaveOut1, 0xFFFFFFFF );//最大化音量,如有需要再编写声音调节子程序
	for(int i=0;i<MAX_BUFFER_NUM_OUT;i++)
	{
	//	memset(&m_pWaveHdrOut[i], 0, sizeof(WAVEHDR) );
//		memset(m_pWaveHdrOut[i].lpData, 0, sizeof(m_pWaveHdrOut[i].lpData) );

#ifdef PRINTF_RECEIVE
					printf("event1-");
#endif;
		//WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);//无限等待PCM数据产生事件，才开始播放.
#ifdef PRINTF_RECEIVE
					printf("event\n");
#endif;
//		//EventPCMReceived.ResetEvent();

//					AudioOutBuf2=pPCMForAudioOutQueue.front();// 从队列中取出一个数据包
			//pBufferOut[BufferOutNo]=(PBYTE)AudioOutBuf2.speech;

			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf1[i][counttmp2]=AudioOutBuf_1.speech[counttmp2];          //将解包出的PCM数据装入结构体AudioDecBuf
			}

			pBufferOut1[BufferOutNo1]=(PBYTE)speechtmp1;
					
		m_pWaveHdrOut1[i].lpData=(LPSTR)speechBuf1[i];  //typedef CHAR* LPSTR
//		pPCMForAudioOutQueue.pop();
		m_pWaveHdrOut1[i].dwBufferLength = 2*MAX_BUFFER_SIZE_OUT;//dwRecDataLength

		m_pWaveHdrOut1[i].dwBytesRecorded=0;
		m_pWaveHdrOut1[i].dwUser=0;
		m_pWaveHdrOut1[i].dwFlags=0;//m_pWaveHdrOut[i].dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
		m_pWaveHdrOut1[i].dwLoops=1;
		m_pWaveHdrOut1[i].lpNext=NULL;
		m_pWaveHdrOut1[i].reserved= 0;

		waveOutPrepareHeader( m_hWaveOut1, &m_pWaveHdrOut1[i], sizeof(WAVEHDR) );	//准备3个缓冲区
		if(MMSYSERR_NOERROR==waveOutWrite( m_hWaveOut1, &m_pWaveHdrOut1[i], sizeof(WAVEHDR) ))
		{
#ifdef PRINTF_RECEIVE
			printf("MMSYSERR_NOERROR");
#endif
		};			//写入3个缓冲区
#ifdef PRINTF_RECEIVE
		printf("out!!!\n");
#endif;
	}
	waveOutRestart(m_hWaveOut1);

}

bool __stdcall AudioStop1()
{	
	waveOutReset(m_hWaveOut1);//停止输出
	waveOutClose(m_hWaveOut1);//关闭设备
    return true;

}
