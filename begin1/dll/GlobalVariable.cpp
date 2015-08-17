#include "stdafx.h"
#include "GlobalVariable.h"



// ******************************************************************************************* //
// 

queue <PBYTE> pDataBufferQueque;					//�������Queue1, [�����ɼ�]�߳���[���롢���]�߳� ����
queue <AMROutBuf> pSocketSendQueque;				//�������Queue2, [���롢���]�߳� �� [UDP Socket����]�߳� ����

queue <AMROutBuf> pRecvfromBufferQueue_1;				//�������Queue3, ����[UDP Socket����]�߳� �� [�������]�߳� ����
queue <AudioOutBuf> pPCMForAudioOutQueue_1;			//�������Queue4, ����[�������]�߳� �� [��Ƶ����]�߳� ����
queue <AMROutBuf> pRecvfromBufferQueue_2;				//�������Queue3, ����[UDP Socket����]�߳� �� [�������]�߳� ����
queue <AudioOutBuf> pPCMForAudioOutQueue_2;			//�������Queue4, ����[�������]�߳� �� [��Ƶ����]�߳� ����

queue <Video_H264> pSocketRcvVideoDecQueue;			//�������Queue5, ����[UDP Socket����]�߳� �� [�������]�߳� ����
queue <Video_YUV420> pVideoDecPlayQueue;			//�������Queue6, ����[�������]�߳� �� [��Ƶ����]�߳� ����

queue <Video_YUV420> pVideoSampleEncQueue;
queue <Video_H264> pVideoEncodeAndSendQueue;

CCriticalSection critical_section1;					//����һ���ٽ�������֤�߳�ͬ��������ͬʱ�Զ��в�����
CCriticalSection critical_section2;

CCriticalSection critical_section3_1;
CCriticalSection critical_section4_1;
CCriticalSection critical_section3_2;
CCriticalSection critical_section4_2;

CCriticalSection critical_section5;
CCriticalSection critical_section6;

CCriticalSection critical_section7;
CCriticalSection critical_section8;

// 
// ******************************************************************************************* //

CEvent EventPCMReceived;			//�¼��������ڶ��߳�ͬ��(FALSE,FALSE,NULL,NULL)
CEvent EventYUVReceived;

PBYTE pTempPCMS,pTempPCMR;
DWORD TempLenS,TempLenR;

CEvent EventPCMS;
BOOL PCMSendSuccessFlag;
CEvent EventPCMR;
BOOL PCMRecSuccessFlag;


PBYTE pTempYUVS;
DWORD TempLenYUVS;

CEvent EventRecodYUVS;
BOOL YUVSmpRecordFlag;

PBYTE pTempYUVD;
DWORD TempLenYUVD;

CEvent EventRecodYUVD;
BOOL YUVRcvRecordFlag;

// ******************************************************************************************* //
// ȫ�ֱ�������

// Video Display for Delphi
DWORD HANDLE_DISPLAY;
int coordinate_x;
int coordinate_y;
int wsize;
int hsize;

// Video Display for DelphiA
DWORD AHANDLE_DISPLAY;
int Acoordinate_x;
int Acoordinate_y;
int Awsize;
int Ahsize;


// RECORD and REPLAY FILE
char* AudioFilePathandName;
char* VideoFilePathandName;

// SGW IP and PORT
char* SGW_IP;
unsigned short SGW_PORT_Audio;
unsigned short SGW_PORT_Video;

// LOCAL IP and PORT
char* LOCAL_IP;
unsigned short LOCAL_RCV_PORT_Audio;
unsigned short LOCAL_RCV_PORT_Video;

unsigned short LOCAL_SEND_PORT_Audio;
unsigned short LOCAL_SEND_PORT_Video;

// DEST IP and PORT
char* DEST_IP;
unsigned short DEST_PORT_Audio;
unsigned short DEST_PORT_Video;

// DEST AudioIPID and VideoIPID
unsigned long DEST_IPID_Audio;
unsigned long DEST_IPID_Video;

// LOCAL AudioIPID and VideoIPID
unsigned long LOCAL_IPID_Audio;
unsigned long LOCAL_IPID_Video;


//char* SendDestIP;
//char* MCASTADDR;
//
//unsigned short AudioDestPORT;
//unsigned short AudioLocalSendPORT;
//unsigned short AudioLocalRcvPORT;
//
//unsigned short VideoDestPORT;
//unsigned short VideoLocalSendPORT;
//unsigned short VideoLocalRcvPORT;
//
//unsigned long Audio_ipid;
//unsigned long Video_ipid;


// ȫ�ֱ�������
// ******************************************************************************************* //


/*���������*/
/////////////////////////////////////////2012.5.4////////////////////////////////////////
Mode My_Mode=MR475;
AMR m_AMR(My_Mode);	//AMR��
/////////////////////////////////////////////////////////////////////////////////////////

//AMR m_AMR(MR475);	//AMR��
//package m_package;	//�����
config m_config;	//��ý��������(IP��Port���ã��ļ�·�����ļ�������)

BOOL AudioRecordFlag = FALSE;
BOOL VideoRecordFlag = FALSE;

SQTS SQTScontrol;		// ��Ƶ����Ƶ�Ŀ�����Ϣ

BOOL SynchronousFlag = FALSE;
BOOL CameraNewOpen = TRUE;

BOOL PLAYRCVONLY = FALSE;

// ********************************************************************************* //
// for open and close audio send
BOOL AudioSampleFlag = FALSE;
BOOL AudioEncFlag = FALSE;
BOOL AudioSendFlag = FALSE;

//CEvent EventAudioSampleEnd;
//CEvent EventAudioEncEnd;
//CEvent EventAudioSendEnd;

// for open and close audio receive
BOOL AudioRcvFlag = FALSE;
BOOL AudioDecFlag = FALSE;
BOOL AudioPlayFlag = FALSE;

//CEvent EventAudioRcvEnd;
//CEvent EventAudioDecEnd;
//CEvent EventAudioPlayEnd;

// for open and close video send
BOOL VideoSampleFlag = FALSE;
BOOL VideoEncodFlag = FALSE;
BOOL VideoSendFlag = FALSE;

CEvent EventVideoSampleEnd;
CEvent EventVideoEncodEnd;
CEvent EventVideoSendEnd;

// for open and close video receive
BOOL VideoRcvFlag = FALSE;
BOOL VideoDecFlag = FALSE;
BOOL VideoPlayFlag = FALSE;



CEvent EventAudioRecord;
CEvent EventVideoRecord;
//*****************LDS���

//CString M4IP1="198.4.4.10";//LDS���for showing UDP IP
CString M4IP1;//LDS���for showing UDP IP
CString M4IP2;
CString M4IP3;
CString M4IP4;


CString M4PORT1;//LDS���for showing UDP PORT
CString M4PORT2;
CString M4PORT3;
CString M4PORT4;

//int R_flag=0;//LDS���Ϊ�ж�UDP���Ƿ���ջ
int Acess_Num=0;//LDS���Ϊ�ж������ȴ���·����
int tab_Mode=0;
int H_local=0;