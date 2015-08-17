// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "SocketThread.h"
#include "AudioSampleEncThread.h"
#include "AudioDecPlayThread1.h"
#include "AudioDecPlayThread2.h"
#include "RecordReplayThread.h"
#include "VideoSampleEncThread.h"
#include "VideoDecDisplayThread.h"
#define WM_TERM_THREAD 1023

#undef inline
#undef DECLARE_ALIGNED
#undef snprintf
#undef IS_INTRA
#undef IS_SKIP
#undef IS_DIRECT



#include <process.h>


/*线程句柄定义*/
	HANDLE hThreadAudioSample;
	HANDLE hThreadAMREncodeAndPackage;
	HANDLE hThreadAudioUDPSocketSend;

	HANDLE hThreadAudioUDPSocketReceive;   
	HANDLE hThreadDepackandAMRDecode1;
	HANDLE hThreadAudioPlay1;
	HANDLE hThreadDepackandAMRDecode2;
	HANDLE hThreadAudioPlay2;


	HANDLE hThreadRecordAudioFile;
	HANDLE hThreadReplayAudioFile;

	HANDLE hThreadVideoSample;
	HANDLE hThreadH264EncodeAndPackage;
	HANDLE hThreadVideoUDPSocketSend;

	HANDLE hThreadVideoUDPSocketReceive;
	HANDLE hThreadDepackand264Decode;
	HANDLE hThreadVideoPlay;

	HANDLE hThreadRecordVideoFile;
	HANDLE hThreadReplayVideoFile;

/*线程ID定义*/
	unsigned int dwThreadIDAudioSample;
	unsigned int dwThreadIDAMREncodeAndPackage;
	unsigned int dwThreadIDAudioUDPSocketSend;

	unsigned int dwThreadIDAudioUDPSocketReceive; 
	unsigned int dwThreadIDDepackandAMRDecode1;
		unsigned int dwThreadIDDepackandAMRDecode2;
	unsigned int dwThreadIDAudioPlay1;
		unsigned int dwThreadIDAudioPlay2;

	unsigned int dwThreadIDRecordAudioFile;
    unsigned int dwThreadIDReplayAudioFile;

	unsigned int dwThreadIDVideoSample;
	unsigned int dwThreadIDH264EncodeAndPackage;
	unsigned int dwThreadIDVideoUDPSocketSend;

	unsigned int dwThreadIDVideoUDPSocketReceive; 
	unsigned int dwThreadDepackand264Decode;
	unsigned int dwThreadVideoPlay;

	unsigned int dwThreadIDRecordVideoFile;
	unsigned int dwThreadIDReplayVideoFile;

// ******************************************************************************************** //


int PASCAL ConfigVideoDisplayerSet(DWORD HANDLE_DISPLAY_Delphi,
									 int coordinate_x_Delphi, int coordinate_y_Delphi,
									 int wsize_Delphi, int hsize_Delphi)
{
	m_config.Video_displayer_set( HANDLE_DISPLAY_Delphi,
		coordinate_x_Delphi, coordinate_y_Delphi,
		wsize_Delphi, hsize_Delphi );
	return 0;
}

int PASCAL ConfigVideoDisplayerSetA(DWORD AHANDLE_DISPLAY_Delphi,
									 int Acoordinate_x_Delphi, int Acoordinate_y_Delphi,
									 int Awsize_Delphi, int Ahsize_Delphi)
{
	m_config.Video_displayer_setA( AHANDLE_DISPLAY_Delphi,
		Acoordinate_x_Delphi, Acoordinate_y_Delphi,
		Awsize_Delphi, Ahsize_Delphi );
	return 0;
}




int PASCAL ConfigSGWIPandPort(char* SGW_IP_Delphi,
							  unsigned short SGW_PORT_Audio_Delphi,unsigned short SGW_PORT_Video_Delphi)
{
	m_config.SetSGWIPandPORT( SGW_IP_Delphi,
		SGW_PORT_Audio_Delphi,
		SGW_PORT_Video_Delphi );
	return 0;
}

int PASCAL ConfigLOCALIPandPort(char* LOCAL_IP_Delphi,
								unsigned short LOCAL_RCV_PORT_Audio_Delphi,
								unsigned short LOCAL_SEND_PORT_Audio_Delphi,unsigned short LOCAL_RCV_PORT_Video_Delphi,
								unsigned short LOCAL_SEND_PORT_Video_Delphi)
{
	m_config.SetLOCALIPandPORT( LOCAL_IP_Delphi,
		LOCAL_RCV_PORT_Audio_Delphi,
		LOCAL_SEND_PORT_Audio_Delphi,LOCAL_RCV_PORT_Video_Delphi,
		LOCAL_SEND_PORT_Video_Delphi);
	return 0;
}

int PASCAL ConfigDESTIPandPort(char* DEST_IP_Delphi,
							   unsigned short DEST_PORT_Audio_Delphi, unsigned short DEST_PORT_Video_Delphi)
{
	m_config.SetDESTIPandPORT( DEST_IP_Delphi,
		DEST_PORT_Audio_Delphi, DEST_PORT_Video_Delphi );
	return 0;
}

int PASCAL ConfigLOCALIPID( unsigned long LOCAL_IPID_Audio_Delphi, unsigned long LOCAL_IPID_Video_Delphi )
{
	m_config.SetLOCALIPID( LOCAL_IPID_Audio_Delphi, LOCAL_IPID_Video_Delphi );
	return 0;
}

int PASCAL ConfigDESTIPID( unsigned long DEST_IPID_Audio_Delphi, unsigned long DEST_IPID_Video_Delphi )
{
	m_config.SetDESTIPID( DEST_IPID_Audio_Delphi, DEST_IPID_Video_Delphi );
	return 0;
}

// ******************************************************************************************** //

int PASCAL OpenAudioSend()
{
	hThreadAudioSample			= (HANDLE)_beginthreadex(NULL,0,&AudioSampleThread,NULL,0,&dwThreadIDAudioSample);
	hThreadAMREncodeAndPackage	= (HANDLE)_beginthreadex(NULL,0,&AMREncodeAndPackageThread,NULL,0,&dwThreadIDAMREncodeAndPackage);
	return 0;
}
int PASCAL OpenAudioSocketSend()
{
	hThreadAudioUDPSocketSend	= (HANDLE)_beginthreadex(NULL,0,&AudioUDPSocketSendThread,NULL,0,&dwThreadIDAudioUDPSocketSend);
	return 0;
}
int PASCAL CloseAudioSocketSend()
{
	Sleep(10);//必要的延时
PostThreadMessage(dwThreadIDAudioUDPSocketSend,WM_TERM_THREAD,0,0);
WaitForSingleObject(hThreadAudioUDPSocketSend,INFINITE);
CloseHandle(hThreadAudioUDPSocketSend);
	return 0;
}
// ******************************************************************************************** //

int PASCAL OpenAudioReceive1()
{
	//hThreadAudioUDPSocketReceive	= (HANDLE)_beginthreadex(NULL,0,&AudioUDPSocketReceiveThread,NULL,0,&dwThreadIDAudioUDPSocketReceive);
	hThreadDepackandAMRDecode1		= (HANDLE)_beginthreadex(NULL,0,&DepackandAMRDecodeThread1,NULL,0,&dwThreadIDDepackandAMRDecode1);
	hThreadAudioPlay1				= (HANDLE)_beginthreadex(NULL,0,&AudioPlayThread1,NULL,0,&dwThreadIDAudioPlay1);
//	hThreadDepackandAMRDecode2		= (HANDLE)_beginthreadex(NULL,0,&DepackandAMRDecodeThread2,NULL,0,&dwThreadIDDepackandAMRDecode2);
	//hThreadAudioPlay2				= (HANDLE)_beginthreadex(NULL,0,&AudioPlayThread2,NULL,0,&dwThreadIDAudioPlay2);
	return 0;
}
int PASCAL OpenAudioReceive2()
{
	//hThreadAudioUDPSocketReceive	= (HANDLE)_beginthreadex(NULL,0,&AudioUDPSocketReceiveThread,NULL,0,&dwThreadIDAudioUDPSocketReceive);
	hThreadDepackandAMRDecode2		= (HANDLE)_beginthreadex(NULL,0,&DepackandAMRDecodeThread1,NULL,0,&dwThreadIDDepackandAMRDecode1);
	hThreadAudioPlay2				= (HANDLE)_beginthreadex(NULL,0,&AudioPlayThread1,NULL,0,&dwThreadIDAudioPlay1);
//	hThreadDepackandAMRDecode2		= (HANDLE)_beginthreadex(NULL,0,&DepackandAMRDecodeThread2,NULL,0,&dwThreadIDDepackandAMRDecode2);
	//hThreadAudioPlay2				= (HANDLE)_beginthreadex(NULL,0,&AudioPlayThread2,NULL,0,&dwThreadIDAudioPlay2);
	return 0;
}
//******添加结束
int PASCAL OpenAudioSocketReceive()
{
hThreadAudioUDPSocketReceive	= (HANDLE)_beginthreadex(NULL,0,&AudioUDPSocketReceiveThread,NULL,0,&dwThreadIDAudioUDPSocketReceive);
return 0;	
}

int PASCAL CloseAudioSocketReceive()
{
	Sleep(10);//必要的延时
PostThreadMessage(dwThreadIDAudioUDPSocketReceive,WM_TERM_THREAD,0,0);
WaitForSingleObject(hThreadAudioUDPSocketReceive,INFINITE);
CloseHandle(hThreadAudioUDPSocketReceive);
	return 0;
}


// ******************************************************************************************** //

int PASCAL OpenVideoSend()
{
	VideoSendFlag = TRUE;
	VideoEncodFlag = TRUE;
	VideoSampleFlag = TRUE;
	return 0;
}



// ******************************************************************************************** //

int PASCAL OpenVideoReceive()
{ VideoPlayFlag = TRUE;
VideoDecFlag = TRUE;
VideoRcvFlag = TRUE;
	return 0;
}

int PASCAL CloseVideoSend()
{
	VideoSendFlag = false;
	VideoEncodFlag = false;
	VideoSampleFlag = false;
	return 0;
}



// ******************************************************************************************** //

int PASCAL CloseVideoReceive()
{ VideoPlayFlag = false;
VideoDecFlag = false;
VideoRcvFlag = false;
	return 0;
}

int PASCAL OpenVideoSocketSend()
{
	hThreadVideoUDPSocketSend	= (HANDLE)_beginthreadex(NULL, 0, &VideoUDPSocketSendThread, NULL, 0, &dwThreadIDVideoUDPSocketSend);
	return 0;
}
int PASCAL CloseVideoSocketSend()
{  	
	Sleep(10);//必要的延时
PostThreadMessage(dwThreadIDVideoUDPSocketSend,WM_TERM_THREAD,0,0);
WaitForSingleObject(hThreadVideoUDPSocketSend,INFINITE);
//CloseHandle(hThreadVideoUDPSocketSend);
	
	return 0;
}

int PASCAL OpenVideoSocketReceive()
{
	hThreadVideoUDPSocketReceive= (HANDLE)_beginthreadex(NULL, 0, &VideoUDPSocketReceiveThread, NULL, 0, &dwThreadIDVideoUDPSocketReceive);	
	return 0;
}

int PASCAL CloseVideoSocketReceive()
{
	Sleep(10);//必要的延时
PostThreadMessage(dwThreadIDVideoUDPSocketReceive,WM_TERM_THREAD,0,0);
WaitForSingleObject(hThreadVideoUDPSocketReceive,INFINITE);
//CloseHandle(hThreadVideoUDPSocketReceive);
	return 0;
}

// ******************************************************************************************** //

//int PASCAL OpenListen()
//{
//	hThreadAudioUDPSocketReceive	= (HANDLE)_beginthreadex(NULL,0,&AudioUDPSocketReceiveThread,NULL,0,&dwThreadIDAudioUDPSocketReceive);
//	hThreadDepackandAMRDecode		= (HANDLE)_beginthreadex(NULL,0,&DepackandAMRDecodeThread,NULL,0,&dwThreadIDDepackandAMRDecode);
//	hThreadAudioPlay				= (HANDLE)_beginthreadex(NULL,0,&AudioPlayThread,NULL,0,&dwThreadIDAudioPlay);
//	return 0;
//}
//int PASCAL CloseListen()
//{
//	TerminateThread(hThreadAudioUDPSocketReceive,NULL);
//	TerminateThread(hThreadDepackandAMRDecode,NULL);
//	TerminateThread(hThreadAudioPlay,NULL);
//	return 0;
//}

// ******************************************************************************************** //

int PASCAL OpenAudioRecord(char* AudioFilePathandName_Delphi)
{
	m_config.SetAudioFilePathandName(AudioFilePathandName_Delphi);
	hThreadRecordAudioFile = (HANDLE)_beginthreadex(NULL,0,&RecordWavFileThread,NULL,0,&dwThreadIDRecordAudioFile);

	return 0;
}
int PASCAL CloseAudioRecord()
{
	AudioRecordFlag = FALSE;
	TerminateThread(hThreadRecordAudioFile,NULL);
	return 0;
}

// ******************************************************************************************** //

int PASCAL OpenVideoRecord(char* VideoFilePathandName_Delphi)
{
	m_config.SetVideoFilePathandName(VideoFilePathandName_Delphi);
	hThreadRecordVideoFile = (HANDLE)_beginthreadex(NULL,0,&VideoRecordThread,NULL,0,&dwThreadIDRecordVideoFile);
	return 0;
}

int PASCAL CloseVideoRecord()
{
	TerminateThread(hThreadRecordVideoFile,NULL);
	return 0;
}

// ******************************************************************************************** //

int PASCAL OpenVideoReplay(char* VideoFilePathandName_Delphi)
{
	m_config.SetVideoFilePathandName(VideoFilePathandName_Delphi);
	hThreadReplayVideoFile	= (HANDLE)_beginthreadex(NULL,0,&VideoReplayThread,NULL,0,&dwThreadIDReplayVideoFile);
	hThreadVideoPlay		= (HANDLE)_beginthreadex(NULL, 0, &VideoPlayThread, NULL, 0, &dwThreadVideoPlay);
	return 0;
}

int PASCAL CloseVideoReplay()
{
	TerminateThread(hThreadReplayVideoFile,NULL);
	TerminateThread(hThreadVideoPlay, NULL);
	return 0;
}

int PASCAL OpenAudioReplay(char* AudioFilePathandName_Delphi)
{
	m_config.SetAudioFilePathandName(AudioFilePathandName_Delphi);
	hThreadReplayAudioFile	= (HANDLE)_beginthreadex(NULL,0,&AudioReplayThread,NULL,0,&dwThreadIDReplayAudioFile);
	hThreadAudioPlay1		= (HANDLE)_beginthreadex(NULL, 0, &AudioPlayThread1, NULL, 0, &dwThreadIDAudioPlay1);
//	hThreadAudioPlay2		= (HANDLE)_beginthreadex(NULL, 0, &AudioPlayThread2, NULL, 0, &dwThreadVideoPlay);
	return 0;
}

int PASCAL CloseAudioReplay()
{
	TerminateThread(hThreadReplayAudioFile,NULL);
	TerminateThread(hThreadAudioPlay1, NULL);
	TerminateThread(hThreadAudioPlay2, NULL);
	return 0;
}

int PASCAL SystemInti()
{
	unsigned long returNum = STILL_ACTIVE;
	hThreadVideoSample			= (HANDLE)_beginthreadex(NULL, 0, &VideoSampleThread, NULL, 0, &dwThreadIDVideoSample);
	hThreadVideoPlay			= (HANDLE)_beginthreadex(NULL, 0, &VideoPlayThread, NULL, 0, &dwThreadVideoPlay);
	hThreadDepackand264Decode	= (HANDLE)_beginthreadex(NULL, 0, &Depackand264DecodeThread, NULL, 0, &dwThreadDepackand264Decode);
	hThreadH264EncodeAndPackage	= (HANDLE)_beginthreadex(NULL, 0, &H264EncodeAndPackageThread, NULL, 0, &dwThreadIDH264EncodeAndPackage);

	return 0;
}
