#include "stdafx.h"
#include "GlobalVariable.h"



// ******************************************************************************************* //
// 

queue <PBYTE> pDataBufferQueque;					//缓冲队列Queue1, [语音采集]线程与[编码、封包]线程 缓冲
queue <AMROutBuf> pSocketSendQueque;				//缓冲队列Queue2, [编码、封包]线程 与 [UDP Socket发送]线程 缓冲

queue <AMROutBuf> pRecvfromBufferQueue_1;				//缓冲队列Queue3, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
queue <AudioOutBuf> pPCMForAudioOutQueue_1;			//缓冲队列Queue4, 用于[拆包解码]线程 与 [音频播放]线程 缓冲
queue <AMROutBuf> pRecvfromBufferQueue_2;				//缓冲队列Queue3, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
queue <AudioOutBuf> pPCMForAudioOutQueue_2;			//缓冲队列Queue4, 用于[拆包解码]线程 与 [音频播放]线程 缓冲

queue <Video_H264> pSocketRcvVideoDecQueue;			//缓冲队列Queue5, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
queue <Video_YUV420> pVideoDecPlayQueue;			//缓冲队列Queue6, 用于[拆包解码]线程 与 [视频播放]线程 缓冲

queue <Video_YUV420> pVideoSampleEncQueue;
queue <Video_H264> pVideoEncodeAndSendQueue;

CCriticalSection critical_section1;					//定义一个临界区，保证线程同步，避免同时对队列操作。
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

CEvent EventPCMReceived;			//事件对象，用于多线程同步(FALSE,FALSE,NULL,NULL)
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
// 全局变量定义

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


// 全局变量定义
// ******************************************************************************************* //


/*类对象声明*/
/////////////////////////////////////////2012.5.4////////////////////////////////////////
Mode My_Mode=MR475;
AMR m_AMR(My_Mode);	//AMR类
/////////////////////////////////////////////////////////////////////////////////////////

//AMR m_AMR(MR475);	//AMR类
//package m_package;	//封包类
config m_config;	//多媒体配置类(IP与Port设置，文件路径与文件名设置)

BOOL AudioRecordFlag = FALSE;
BOOL VideoRecordFlag = FALSE;

SQTS SQTScontrol;		// 音频给视频的控制信息

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
//*****************LDS添加

//CString M4IP1="198.4.4.10";//LDS添加for showing UDP IP
CString M4IP1;//LDS添加for showing UDP IP
CString M4IP2;
CString M4IP3;
CString M4IP4;


CString M4PORT1;//LDS添加for showing UDP PORT
CString M4PORT2;
CString M4PORT3;
CString M4PORT4;

//int R_flag=0;//LDS添加为判断UDP包是否入栈
int Acess_Num=0;//LDS添加为判断哪条等待线路接入
int tab_Mode=0;
int H_local=0;