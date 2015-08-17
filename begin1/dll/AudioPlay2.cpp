/*2010-05-10
增加： pPCMForAudioOutQueue的关键区同步
变量定义：
static BOOL pPCMForAudioOutQueueEmpty;
增加关键区同步 pPCMForAudioOutQueue
*/
#include "stdafx.h"
#include "AudioPlay2.h"

void CALLBACK WaveOutProc2(HWAVEOUT m_hWaveOut2, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2)
{
	switch(uMsg)
	{

		case WOM_OPEN: 
			bDevAudioOutOpen2=true;
#ifdef PRINTF_SEND
			cout<<"\nAudio Device is OPEN!"<<endl;
#endif
			break;
		case WOM_CLOSE:
			bDevAudioOutOpen2=false;
#ifdef PRINTF_RECEIVE
			cout<<"\nAudio Device is CLOSE!"<<endl;
#endif
			break;
		case WOM_DONE:
#ifdef PRINTF_RECEIVE
			cout<<"WOM_DATA_DONE"<<testcount12<<endl;
#endif
			testcount12++;
/*11.16改动：输出当前播放的那一帧PCM数据对应的sq和ts值，即在sqts数组的当前指针的下一个 （开始）*/

		    if(BufferOutNo2 == MAX_BUFFER_NUM_OUT-1)  //MAX_BUFFER_NUM_OUT=5
			{
				//cout<<"Sequence number "<<sqts[0].sq<<" - "<<"Timestamp number "<<sqts[0].ts<<"\n";
				SQTScontrol.sq = sqts2[0].sq;
				SQTScontrol.ts = sqts2[0].ts;
			}
			else
			{
				//cout<<"Sequence number "<<sqts[BufferOutNo+1].sq<<" - "<<"Timestamp number "<<sqts[BufferOutNo+1].ts<<"\n";
				SQTScontrol.sq = sqts2[BufferOutNo2+1].sq;
				SQTScontrol.ts = sqts2[BufferOutNo2+1].ts;
			}


/*11.16改动：输出当前播放的那一帧PCM数据对应的sq和ts值，即在sqts数组的当前指针的下一个  （结束）*/

		/*方法二：开始*/
			PWAVEHDR pWaveHeader=((PWAVEHDR)dwParam1);//Step1: 获取已经播放完毕的WAVEHDR
//			cout<<pWaveHeader<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n"; //输出缓冲区的地址，调试用
			waveOutUnprepareHeader(m_hWaveOut2,pWaveHeader,sizeof(WAVEHDR)); //Step2: 释放声卡设备缓存
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
		static BOOL pPCMForAudioOutQueue_2Empty;
		critical_section4_2.Lock();
		pPCMForAudioOutQueue_2Empty=pPCMForAudioOutQueue_2.empty();
		critical_section4_2.Unlock();
		if(!pPCMForAudioOutQueue_2Empty)
		{
			
			critical_section4_2.Lock();
			AudioOutBuf_2=pPCMForAudioOutQueue_2.front();// 从队列中取出一个数据包
			pPCMForAudioOutQueue_2.pop();
			critical_section4_2.Unlock();
	
			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf2[BufferOutNo2][counttmp2]=AudioOutBuf_2.speech[counttmp2];          //将解包出的PCM数据装入结构体AudioDecBuf
			}
		}
		else
		{
			for(int counttmp3=0;counttmp3<160;counttmp3++)
			{
				speechBuf2[BufferOutNo2][counttmp3]=0;          //静音，填充0
			}
		}

	/*11.16改动：新抓入的一阵AudioOutBuf数据在被拆解时顺便把其中的序列号、时间戳信息提取出来赋给sqts数组记录  （结束）*/
	
		sqts2[BufferOutNo2].sq = AudioOutBuf_2.sequencenum;
		sqts2[BufferOutNo2].ts = AudioOutBuf_2.timestamp;


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


			(*pWaveHeader).lpData =(LPSTR)speechBuf2[BufferOutNo2];
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


			waveOutPrepareHeader( m_hWaveOut2, &m_pWaveHdrOut2[BufferOutNo2], sizeof(WAVEHDR) );
			waveOutWrite( m_hWaveOut2, &m_pWaveHdrOut2[BufferOutNo2], sizeof(WAVEHDR) );
			
			BufferOutNo2++;
			if(BufferOutNo2==MAX_BUFFER_NUM_OUT) {BufferOutNo2=0;}


	//		}
			
			break;
		/*方法二：结束*/


	}
}
void WINAPI InitAudioOut2()
{
    bDevAudioOutOpen2=false;
	BufferOutNo2=0;

	m_soundFormat_out2.wFormatTag=WAVE_FORMAT_PCM;
	m_soundFormat_out2.nChannels=1;
	m_soundFormat_out2.nSamplesPerSec=8000;
	m_soundFormat_out2.nAvgBytesPerSec=16000;
	m_soundFormat_out2.nBlockAlign=2;
	m_soundFormat_out2.cbSize=0;
	m_soundFormat_out2.wBitsPerSample=16;     //指定播音格式
	
	waveOutOpen(&m_hWaveOut2,WAVE_MAPPER,&m_soundFormat_out2,(DWORD)WaveOutProc2,NULL,CALLBACK_FUNCTION);//打开播音设备
	
	waveOutPause(m_hWaveOut2);//先暂停输出，将所有缓冲区填充满后再输出

	waveOutSetVolume(m_hWaveOut2, 0xFFFFFFFF );//最大化音量,如有需要再编写声音调节子程序
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
				speechBuf2[i][counttmp2]=AudioOutBuf_2.speech[counttmp2];          //将解包出的PCM数据装入结构体AudioDecBuf
			}

			pBufferOut2[BufferOutNo2]=(PBYTE)speechtmp2;
					
		m_pWaveHdrOut2[i].lpData=(LPSTR)speechBuf2[i];  //typedef CHAR* LPSTR
//		pPCMForAudioOutQueue.pop();
		m_pWaveHdrOut2[i].dwBufferLength = 2*MAX_BUFFER_SIZE_OUT;//dwRecDataLength

		m_pWaveHdrOut2[i].dwBytesRecorded=0;
		m_pWaveHdrOut2[i].dwUser=0;
		m_pWaveHdrOut2[i].dwFlags=0;//m_pWaveHdrOut[i].dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
		m_pWaveHdrOut2[i].dwLoops=1;
		m_pWaveHdrOut2[i].lpNext=NULL;
		m_pWaveHdrOut2[i].reserved= 0;

		waveOutPrepareHeader( m_hWaveOut2, &m_pWaveHdrOut2[i], sizeof(WAVEHDR) );	//准备3个缓冲区
		if(MMSYSERR_NOERROR==waveOutWrite( m_hWaveOut2, &m_pWaveHdrOut2[i], sizeof(WAVEHDR) ))
		{
#ifdef PRINTF_RECEIVE
			printf("MMSYSERR_NOERROR");
#endif
		};			//写入3个缓冲区
#ifdef PRINTF_RECEIVE
		printf("out!!!\n");
#endif;
	}
	waveOutRestart(m_hWaveOut2);

}

bool __stdcall AudioStop2()
{	
	waveOutReset(m_hWaveOut2);//停止输出
	waveOutClose(m_hWaveOut2);//关闭设备
    return true;

}
