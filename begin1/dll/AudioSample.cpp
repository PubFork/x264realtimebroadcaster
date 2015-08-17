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
			critical_section1.Lock();//����
			pDataBufferQueque.push(pBufferIn[BufferInNo]);//����֡���ݵ�ָ�뵽����
			critical_section1.Unlock();//����


/*¼����������-��ʼ-*/

/* // 1.0 Դ����
if(bRecordThread==TRUE)
			{
				printf("Send Record Thread Start!");
				pFileSaveBuffer= (PBYTE)realloc (pFileSaveBuffer, dwDataLength+m_pWaveHdrIn[BufferInNo].dwBytesRecorded);
				CopyMemory(pFileSaveBuffer + dwDataLength, (PBYTE)pBufferIn[BufferInNo],m_pWaveHdrIn[BufferInNo].dwBytesRecorded);
				dwDataLength+=m_pWaveHdrIn[BufferInNo].dwBytesRecorded ;
				if(dwDataLength>320*100)//���ÿ100K����һ��
				{
#ifdef PRINTF_RECORD
					printf("send in\n");
#endif;
					PCMSendSuccessFlag=TRUE;  //�ɹ��ɼ������͵�100K����
					TempLenS=dwDataLength;    //���ɼ���������ת�������ļ�����
					pTempPCMS=new BYTE[TempLenS];
					CopyMemory(pTempPCMS,pFileSaveBuffer,dwDataLength);
					EventPCMS.SetEvent();     //֪ͨд¼��д�ļ��߳̿��Կ�ʼ�����ϳɡ�
					dwDataLength=0;
					pFileSaveBuffer=new BYTE[0]; //������д�ķ���PCM���ݣ��ͷ��ڴ�;
#ifdef PRINTF_RECORD
					printf("send out\n");
#endif;
				}
			}

		}
*/
/*¼����������-����-*/
/* 11��22�����ڸĶ���  */ 
			
if(AudioRecordFlag == TRUE)
			{
				// printf("Send Record Thread Start!");
				//  m_pWaveHdrIn[BufferInNo].dwBytesRecorded �Ƕ�ֵ 320��
				pFileSaveBuffer= (PBYTE)realloc (pFileSaveBuffer, dwDataLength+m_pWaveHdrIn[BufferInNo].dwBytesRecorded+8);
				unsigned char zero_a[8]={0};
				memcpy(pFileSaveBuffer + dwDataLength,zero_a ,8);
				CopyMemory(pFileSaveBuffer + dwDataLength + 8, (PBYTE)pBufferIn[BufferInNo],m_pWaveHdrIn[BufferInNo].dwBytesRecorded);




				dwDataLength+=328;

				if(dwDataLength>328*100)//���ÿ100K����һ��
				{
#ifdef PRINTF_RECORD
					printf("send in\n");
#endif;
					PCMSendSuccessFlag=TRUE;  //�ɹ��ɼ������͵�100K����
					TempLenS=dwDataLength;    //���ɼ���������ת�������ļ�����
					pTempPCMS=new BYTE[TempLenS];
					CopyMemory(pTempPCMS,pFileSaveBuffer,dwDataLength);
					EventPCMS.SetEvent();     //֪ͨд¼��д�ļ��߳̿��Կ�ʼ�����ϳɡ�
					dwDataLength=0;
					pFileSaveBuffer=new BYTE[0]; //������д�ķ���PCM���ݣ��ͷ��ڴ�;
#ifdef PRINTF_RECORD
					printf("send out\n");
#endif;
				}
			}

		}


		//׼����һ�������ɼ�������
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
	m_soundFormat.wBitsPerSample=16;     //ָ��¼����ʽ

	waveInOpen(&m_hWaveIn,WAVE_MAPPER, &m_soundFormat,(DWORD)WaveInProc,NULL,CALLBACK_FUNCTION); //��¼���豸
	for(int i=0;i<MAX_BUFFER_NUM;i++)
	{	
		pBufferIn[i]=(PBYTE)malloc(MAX_BUFFER_SIZE);
		m_pWaveHdrIn[i].lpData=(LPSTR)pBufferIn[i];
		m_pWaveHdrIn[i].dwBufferLength=MAX_BUFFER_SIZE;
		m_pWaveHdrIn[i].dwUser=0;
		m_pWaveHdrIn[i].dwBytesRecorded=0;
		m_pWaveHdrIn[i].dwFlags=0;
		waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[i],sizeof(WAVEHDR)); //׼���ڴ��¼��
		waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[i],sizeof(WAVEHDR)); //�����ڴ��
	}
}

void __stdcall Record()
{
	waveInStart(m_hWaveIn);//��ʼ¼��
}

bool __stdcall StopRecord()
{
	waveInStop(m_hWaveIn); //ֹͣ¼��
	waveInReset(m_hWaveIn); //����ڴ��
    return true;
}

