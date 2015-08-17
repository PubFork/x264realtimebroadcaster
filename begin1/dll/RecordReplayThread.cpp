#include "stdafx.h"
#include "RecordReplayThread.h"

unsigned WINAPI RecordWavFileThread(void* pvParam)
{	
	AudioRecordFlag = TRUE;



	//DWORD m_WaveHeaderSize = 38;
	//DWORD m_WaveFormatSize = 18;

    FILE *file_wav;
	fopen_s(&file_wav, AudioFilePathandName, "wb+");//fopen�ᱻ��Ϊ����ȫ��Ӧ�ò��ð�ȫ����fopen_s()


	fclose(file_wav);

	PBYTE pTempPCMforWav;
	DWORD TempPCMforWavLen=0;
	DWORD PCMDataLen=0;

	int MixPCMResult=0;
	while(1)
	{
#ifdef PRINTF_RECORD
		printf("while In\n");
#endif;
		/*û�вɼ������͵�PCM���ݺͽ��յ�PCM����ʱ��һֱ�ȴ�*/
		while(1)
		{
#ifdef PRINTF_RECORD
			printf("...");
#endif;
			if(PCMSendSuccessFlag||PCMRecSuccessFlag) break;
			Sleep(5);
		}
	
		Sleep(1900);




		/*�ɹ��ɼ������͵�PCM���ݣ����ҳɹ��ɼ������յ�����*/

		

		if((PCMSendSuccessFlag==TRUE)&&(PCMRecSuccessFlag==TRUE))
		{
#ifdef PRINTF_RECORD
			printf("�շ���·���ݻ�������...\n"); 
#endif;
			WaitForSingleObject(EventPCMS.m_hObject,INFINITE);
			WaitForSingleObject(EventPCMR.m_hObject,INFINITE);
			TempPCMforWavLen=TempLenR;					//������Խ��յ������ݳ���Ϊ׼
			pTempPCMforWav=new BYTE[TempPCMforWavLen];

			short* pTempPCMS1=(short*)pTempPCMS;
			short* pTempPCMR1=(short*)pTempPCMR;
			short* pTempPCMforWav1=(short*)pTempPCMforWav;
			/*�����㷨: �����ȡ��ֵ;�����Ż��ο����ġ���Ƶ�����л��������������о������������*/
			for(unsigned long i=0;i<TempPCMforWavLen/2;i++)
			{   
				MixPCMResult=*(pTempPCMS1+i)+*(pTempPCMR1+i);
				if(MixPCMResult<-32768)   
					MixPCMResult=-32768;
				else
					if(MixPCMResult>32767)
					{MixPCMResult=32767;}
				*(pTempPCMforWav1+i)=(short)MixPCMResult;
				MixPCMResult=0;
			}
			PCMSendSuccessFlag=FALSE;
			PCMRecSuccessFlag=FALSE;
		}
		else/*�ɹ��ɼ������յ�PCM���ݣ����޷��͵�����*/
		      // ��֤�ɹ� ������
			if((PCMSendSuccessFlag==FALSE)&&(PCMRecSuccessFlag==TRUE))
			{
#ifdef PRINTF_RECORD
				printf("��������д���ļ����޷���");
#endif;
				WaitForSingleObject(EventPCMR.m_hObject,INFINITE);
				TempPCMforWavLen = TempLenR;
				pTempPCMforWav = new BYTE[TempPCMforWavLen];			  
				CopyMemory(pTempPCMforWav, pTempPCMR, TempPCMforWavLen); //����������������ֱ�ӿ������� 
				PCMRecSuccessFlag = FALSE;					   //��λ���ȴ��´βɼ��ɹ�
			}
			else
				/*�ɹ��ɼ������͵�PCM���ݣ����޽��յ�����*/  //  ������
				{
#ifdef PRINTF_RECORD
					printf("��������д���ļ����޽���");
#endif;
					WaitForSingleObject(EventPCMS.m_hObject,INFINITE);
					TempPCMforWavLen=TempLenS;
					pTempPCMforWav=new BYTE[TempPCMforWavLen];
					CopyMemory(pTempPCMforWav,pTempPCMS,TempPCMforWavLen); //����������������ֱ�ӿ�������
					PCMSendSuccessFlag=FALSE;					   //��λ���ȴ��´βɼ��ɹ���
				}

		
	
					
		//  д���ļ�			
		PCMDataLen+=TempPCMforWavLen;//����PCM���ݳ���
		fopen_s(&file_wav,AudioFilePathandName,"rb+");
     


		fseek(file_wav,0,SEEK_END);      //�������PCM����д���ļ�β��
		fwrite(pTempPCMforWav,TempPCMforWavLen, 1, file_wav);

		fflush(file_wav);                //����
		fclose(file_wav);
		delete []pTempPCMS;				 //������д��Ӳ�̣��ͷ��ڴ�
		delete []pTempPCMR;
		delete []pTempPCMforWav;
	}
	return 0;
}


unsigned WINAPI AudioReplayThread(void* pvParam)
{

	 FILE *file_adat;
	 AudioOutBuf pcmreplaybuf;
     PBYTE pReplayPCMbuffer = NULL;
     DWORD LenReplayPCMbuffer;

	 LenReplayPCMbuffer = 328;
	int size;

	if (fopen_s(&file_adat, AudioFilePathandName, "rb") != 0)
	{
		perror("Audio file Read error:\n");
		exit (1);
	}

	printf("Audio Replay Thread Start!\n");
	while(1)
	{
		pReplayPCMbuffer = new BYTE[LenReplayPCMbuffer];
        size = fread(pReplayPCMbuffer, sizeof(char), LenReplayPCMbuffer, file_adat);

        if (size == 0)
            break;
		memcpy((PBYTE)&pcmreplaybuf, pReplayPCMbuffer, LenReplayPCMbuffer);
		//printf("Read frame%d\n",pcmreplaybuf.sequencenum);

		
		critical_section3_1.Lock();
		pPCMForAudioOutQueue_1.push(pcmreplaybuf);
		critical_section3_1.Unlock();

		EventPCMReceived.SetEvent();

		delete []pReplayPCMbuffer;
		Sleep(20); //  �����ĳ�??�� ��ƵΪSleep(40)
	}
	fclose(file_adat);


	return 0;
}


unsigned WINAPI VideoRecordThread(void* pvParam)
{
    FILE *file_video;

	PBYTE pRecodYUVbuffer = NULL;
	DWORD LenRecodYUVbuffer = 0;

	pTempYUVD = new BYTE[MAX_YUV_LEN];

	if (fopen_s(&file_video, VideoFilePathandName, "wb+") != 0)
	{
		perror("Video file found error:\n");
		return 1;
	}
	fclose(file_video);

	VideoRecordFlag = TRUE;
	DWORD RecodNum = 0;
	while (1)
	{
		WaitForSingleObject(EventRecodYUVD.m_hObject,INFINITE);

		LenRecodYUVbuffer = TempLenYUVD;
		pRecodYUVbuffer = new BYTE[LenRecodYUVbuffer];
		memcpy(pRecodYUVbuffer, pTempYUVD, LenRecodYUVbuffer);
		memcpy(&RecodNum, pRecodYUVbuffer, 4);
		//RecodNum++;
		printf("writing video frame%d\n",RecodNum);

		fopen_s(&file_video, VideoFilePathandName, "rb+");
		fseek(file_video,0,SEEK_END);
		fwrite(pRecodYUVbuffer, sizeof(char), LenRecodYUVbuffer, file_video);

		fflush(file_video);
		fclose(file_video);

		delete []pRecodYUVbuffer;
	}

	delete []pTempYUVD;
	VideoRecordFlag = FALSE;
	return 0;
}

unsigned WINAPI VideoReplayThread(void* pvParam)
{
    FILE *file_vdat;
    Video_YUV420	yuvreplaybuf;
	PBYTE pReplayYUVbuffer = NULL;
	DWORD LenReplayYUVbuffer;
	int w = 176;
	int h = 144;
	LenReplayYUVbuffer = w*h*3/2+16;
	int size;

	if (fopen_s(&file_vdat, VideoFilePathandName, "rb") != 0)
	{
		perror("Video file Read error:\n");
		exit (1);
	}

	printf("Video Replay Thread Start!\n");
	while(1)
	{
		pReplayYUVbuffer = new BYTE[LenReplayYUVbuffer];
        size = fread(pReplayYUVbuffer, sizeof(char), LenReplayYUVbuffer, file_vdat);

        if (size == 0)
            break;
		memcpy((PBYTE)&yuvreplaybuf, pReplayYUVbuffer, LenReplayYUVbuffer);
		printf("Read frame%d\n",yuvreplaybuf.sq);

		critical_section2.Lock();
//	pYUVForVideoPlayQueue.push(yuvreplaybuf);
		critical_section2.Unlock();

		EventYUVReceived.SetEvent();

		delete []pReplayYUVbuffer;
		Sleep(40);
	}
	fclose(file_vdat);
	return 0;
}