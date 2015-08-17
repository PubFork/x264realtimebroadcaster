/*2010-05-10
���ӣ� pPCMForAudioOutQueue�Ĺؼ���ͬ��
�������壺
static BOOL pPCMForAudioOutQueueEmpty;
���ӹؼ���ͬ�� pPCMForAudioOutQueue
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
/*11.16�Ķ��������ǰ���ŵ���һ֡PCM���ݶ�Ӧ��sq��tsֵ������sqts����ĵ�ǰָ�����һ�� ����ʼ��*/

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


/*11.16�Ķ��������ǰ���ŵ���һ֡PCM���ݶ�Ӧ��sq��tsֵ������sqts����ĵ�ǰָ�����һ��  ��������*/

		/*����������ʼ*/
			PWAVEHDR pWaveHeader=((PWAVEHDR)dwParam1);//Step1: ��ȡ�Ѿ�������ϵ�WAVEHDR
//			cout<<pWaveHeader<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n"; //����������ĵ�ַ��������
			waveOutUnprepareHeader(m_hWaveOut1,pWaveHeader,sizeof(WAVEHDR)); //Step2: �ͷ������豸����
//			cout<<pWaveHeader<<speechBuf[0]<<speechBuf[1]<<speechBuf[2]<<"\n";
	//		DONE_Count++;
	//		if(DONE_Count==MAX_BUFFER_NUM)
	//		{
  
//			if(pPCMForAudioOutQueue.empty())
//			{
			//printf("event2-");
		   // WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);//���޵ȴ�PCM���ݲ����¼����ſ�ʼ����.
			//printf("event\n");

//				EventPCMReceived.ResetEvent();

//			}
//
			//if(!pPCMForAudioOutQueue.empty())
//			AudioOutBuf2=pPCMForAudioOutQueue.front();// �Ӷ�����ȡ��һ�����ݰ�

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
			AudioOutBuf_1=pPCMForAudioOutQueue_1.front();// �Ӷ�����ȡ��һ�����ݰ�
			pPCMForAudioOutQueue_1.pop();
			critical_section4_1.Unlock();
	
			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf1[BufferOutNo1][counttmp2]=AudioOutBuf_1.speech[counttmp2];          //���������PCM����װ��ṹ��AudioDecBuf
			}
		}
		else
		{
			for(int counttmp3=0;counttmp3<160;counttmp3++)
			{
				speechBuf1[BufferOutNo1][counttmp3]=0;          //���������0
			}
		}

	/*11.16�Ķ�����ץ���һ��AudioOutBuf�����ڱ����ʱ˳������е����кš�ʱ�����Ϣ��ȡ��������sqts�����¼  ��������*/
	
		sqts1[BufferOutNo1].sq = AudioOutBuf_1.sequencenum;
		sqts1[BufferOutNo1].ts = AudioOutBuf_1.timestamp;


/* 11��24�� �Ķ���

	if(!pPCMForAudioOutQueueEmpty)
		{
			
			critical_section4.Lock();
			AudioOutBuf2=pPCMForAudioOutQueue.front();// �Ӷ�����ȡ��һ�����ݰ�
			pPCMForAudioOutQueue.pop();
			critical_section4.Unlock();
	
			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf[BufferOutNo][counttmp2]=AudioOutBuf2.speech[counttmp2];          //���������PCM����װ��ṹ��AudioDecBuf
				
				sqts[BufferOutNo].sq=AudioOutBuf2.sequencenum;
				sqts[BufferOutNo].ts=AudioOutBuf2.timestamp;

			}
		}
		else
		{
			for(int counttmp3=0;counttmp3<160;counttmp3++)
			{
				speechBuf[BufferOutNo][counttmp3]=0;          //���������0
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
		/*������������*/


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
	m_soundFormat_out1.wBitsPerSample=16;     //ָ��������ʽ
	
	waveOutOpen(&m_hWaveOut1,WAVE_MAPPER,&m_soundFormat_out1,(DWORD)WaveOutProc1,NULL,CALLBACK_FUNCTION);//�򿪲����豸
	
	waveOutPause(m_hWaveOut1);//����ͣ����������л�����������������

	waveOutSetVolume(m_hWaveOut1, 0xFFFFFFFF );//�������,������Ҫ�ٱ�д���������ӳ���
	for(int i=0;i<MAX_BUFFER_NUM_OUT;i++)
	{
	//	memset(&m_pWaveHdrOut[i], 0, sizeof(WAVEHDR) );
//		memset(m_pWaveHdrOut[i].lpData, 0, sizeof(m_pWaveHdrOut[i].lpData) );

#ifdef PRINTF_RECEIVE
					printf("event1-");
#endif;
		//WaitForSingleObject(EventPCMReceived.m_hObject,INFINITE);//���޵ȴ�PCM���ݲ����¼����ſ�ʼ����.
#ifdef PRINTF_RECEIVE
					printf("event\n");
#endif;
//		//EventPCMReceived.ResetEvent();

//					AudioOutBuf2=pPCMForAudioOutQueue.front();// �Ӷ�����ȡ��һ�����ݰ�
			//pBufferOut[BufferOutNo]=(PBYTE)AudioOutBuf2.speech;

			for(int counttmp2=0;counttmp2<160;counttmp2++)
			{
				speechBuf1[i][counttmp2]=AudioOutBuf_1.speech[counttmp2];          //���������PCM����װ��ṹ��AudioDecBuf
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

		waveOutPrepareHeader( m_hWaveOut1, &m_pWaveHdrOut1[i], sizeof(WAVEHDR) );	//׼��3��������
		if(MMSYSERR_NOERROR==waveOutWrite( m_hWaveOut1, &m_pWaveHdrOut1[i], sizeof(WAVEHDR) ))
		{
#ifdef PRINTF_RECEIVE
			printf("MMSYSERR_NOERROR");
#endif
		};			//д��3��������
#ifdef PRINTF_RECEIVE
		printf("out!!!\n");
#endif;
	}
	waveOutRestart(m_hWaveOut1);

}

bool __stdcall AudioStop1()
{	
	waveOutReset(m_hWaveOut1);//ֹͣ���
	waveOutClose(m_hWaveOut1);//�ر��豸
    return true;

}
