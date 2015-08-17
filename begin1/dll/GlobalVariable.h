/*
2010-05-10
增加关键区
extern  CCriticalSection critical_section3;//定义一个临界区，保证线程同步，避免同时对队列Queue3操作
extern  CCriticalSection critical_section4;//定义一个临界区，保证线程同步，避免同时对队列Queue4操作

*/
#ifndef _GlobalVariable_H_
#define _GlobalVariable_H_

#ifndef WINVER
#define WINVER    0x0501 // Windows版本为XP, Vista为0x0600 
#endif

#ifndef _AFXDLL
#define _AFXDLL
#endif


#define IP_IN_PACKAGE_MTU 10000
#define IP_IN_PACKAGE_MIN 38

#define SOCKETBUFFERSIZE 200


#define MAX_VIDEO_UDP_PACKET 10000		// UDP收到的数据包大小
#define MAX_264_LEN 10000			// 一帧264视频包的长度
#define MAX_YUV_LEN 60000				// 解码后数据（yuv一帧 width*height）
#define TIME_STEP	2					// 时间增量

#define MAX_VIDEO_LEN 10000			// 一帧264视频包的长度
#define MAX_AUDIO_LEN 200


//#define PRINTF_SEND		//发送调试，注释掉后无输出显示
#define PRINTF_RECORD	//发送调试，注释掉后无输出显示
//#define PRINTF_RECEIVE	//发送调试，注释掉后无输出显示

#include <afxmt.h>	//线程同步类
#include <queue>		//队列类queue声明,需要 using namespace std;
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "config.h"
#include "AMR.h"
//#include "package.h"


using std::cerr;
using std::endl;
using std::cin;
using std::cout;

using std::queue;

// *******************************************************************************************
// 结构体声明

// 用于存放播放的序列号和时间戳
typedef struct
{
	int sq;
	int ts;
}SQTS;

typedef struct dtpu_hdr
{
    unsigned char	bDtpUFirstOct;       /* The First Octect of DTPU Header 0x28*/
    unsigned char	bDtpUMsgType;        /* Message Type */
    unsigned short	wDtpUMsgLen;         /* Message Length */
    unsigned long	dwDtpUIpId;
}DTP_U_HDR;

// Define the IP header
typedef struct ip_hdr
{
	unsigned char  ip_verlen;        // 4-bit IPv4 version
                                     // 4-bit header length (in
                                     // 32-bit words)
	unsigned char	ip_tos;           // IP type of service
	unsigned short	ip_totallength;   // Total length
	unsigned short	ip_id;            // Unique identifier
	unsigned short	ip_offset;        // Fragment offset field
	unsigned char	ip_ttl;           // Time to live
	unsigned char	ip_protocol;      // Protocol(TCP,UDP etc)
	unsigned short	ip_checksum;      // IP checksum
	unsigned long	ip_srcaddr;       // Source address
	unsigned long	ip_destaddr;      // Source address
}IPV4_HDR;

// Define the UDP header
typedef struct udp_hdr
{
	unsigned short src_portno;       // Source port no.
	unsigned short dst_portno;       // Dest. port no.
	unsigned short udp_length;       // Udp packet length
	unsigned short udp_checksum;     // Udp checksum (optional)
}UDP_HDR;

typedef struct
{
	DTP_U_HDR		mDtpuhdr;
	IPV4_HDR		mIphdr;
	UDP_HDR			mUdphdr;
    unsigned char	RTP_data[MAX_VIDEO_LEN];
	unsigned short	data_len;
}Virtual_Video_IP_Package;




typedef struct
{
	DTP_U_HDR		mDtpuhdr;
	IPV4_HDR		mIphdr;
	UDP_HDR			mUdphdr;
    unsigned char	RTP_data[MAX_AUDIO_LEN];
}Virtual_Audio_IP_Package;


typedef struct
{
	int	datalen;
	unsigned char	SocketData[MAX_AUDIO_LEN];
}AudioSocketBuf;

typedef struct
{
	short sequencenum;   
	long timestamp;
	unsigned char AMR_analysis[32];
}AMROutBuf;

typedef struct  //音频输出的结构体 用于压入队列进行排序
{
	int sequencenum;   
	int timestamp;
	short speech[160];
}AudioOutBuf;


typedef struct
{
	int	datalen;
	unsigned char	SocketData[MAX_VIDEO_LEN];
}SocketBuf;  //VideoSocketBuf 改为SocketBuf ，音视频公用 


typedef struct
{
	int	datalen;
	unsigned char	SocketData[MAX_VIDEO_LEN];
}VideoSocketBuf;

typedef struct
{
	int sq;
	int ts;
	int	slicelen;
	unsigned char	slicebuf[MAX_264_LEN];
}Video_H264;

typedef struct
{
	int sq;
	int ts;
	int	width;   
	int	height;
	unsigned char	YUVbuffer[MAX_YUV_LEN];
}Video_YUV420;


// 结构体声明
// ******************************************************************************************* //



extern queue <PBYTE> pDataBufferQueque;					// 缓冲队列Queue1, [语音采集]线程与[编码、封包]线程 缓冲
extern queue <AMROutBuf> pSocketSendQueque;		// 缓冲队列Queue2, [编码、封包]线程 与 [UDP Socket发送]线程 缓冲

extern queue <AMROutBuf> pRecvfromBufferQueue_1;		// 缓冲队列Queue3, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
extern queue <AudioOutBuf> pPCMForAudioOutQueue_1;		// 缓冲队列Queue4, 用于[拆包解码]线程 与 [音频播放]线程 缓冲
extern queue <AMROutBuf> pRecvfromBufferQueue_2;		// 缓冲队列Queue3, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
extern queue <AudioOutBuf> pPCMForAudioOutQueue_2;		// 缓冲队列Queue4, 用于[拆包解码]线程 与 [音频播放]线程 缓冲

extern queue <Video_H264> pSocketRcvVideoDecQueue;			// 缓冲队列Queue5, 用于[UDP Socket接收]线程 与 [拆包解码]线程 缓冲
extern queue <Video_YUV420> pVideoDecPlayQueue;				// 缓冲队列Queue6, 用于[拆包解码]线程 与 [视频播放]线程 缓冲

extern queue <Video_YUV420> pVideoSampleEncQueue;
extern queue <Video_H264> pVideoEncodeAndSendQueue;



extern  CCriticalSection critical_section1;// 定义一个临界区，保证线程同步，避免同时对队列Queue1操作
extern  CCriticalSection critical_section2;// 定义一个临界区，保证线程同步，避免同时对队列Queue2操作

extern  CCriticalSection critical_section3_1;
extern  CCriticalSection critical_section4_1;
extern  CCriticalSection critical_section3_2;
extern  CCriticalSection critical_section4_2;


extern	CCriticalSection critical_section5;
extern	CCriticalSection critical_section6;

extern	CCriticalSection critical_section7;
extern	CCriticalSection critical_section8;

// ******************************************************************************************* //

extern  CEvent EventPCMReceived;//事件对象，用于多线程同步(FALSE,FALSE,NULL,NULL)
extern	CEvent EventYUVReceived;


extern  PBYTE pTempPCMS,pTempPCMR;
extern  DWORD TempLenS,TempLenR;

extern  CEvent EventPCMS;
extern  BOOL PCMSendSuccessFlag;
extern  CEvent EventPCMR;
extern  BOOL PCMRecSuccessFlag;


extern PBYTE pTempYUVS;
extern DWORD TempLenYUVS;

extern CEvent EventRecodYUVS;
extern BOOL YUVSmpRecordFlag;

extern  PBYTE pTempYUVD;
extern  DWORD TempLenYUVD;

extern  CEvent EventRecodYUVD;
extern  BOOL YUVRcvRecordFlag;

// ******************************************************************************************* //
// 全局变量定义

// Video Display for Delphi
extern DWORD HANDLE_DISPLAY;
extern int coordinate_x;
extern int coordinate_y;
extern int wsize;
extern int hsize;

extern DWORD AHANDLE_DISPLAY;
extern int Acoordinate_x;
extern int Acoordinate_y;
extern int Awsize;
extern int Ahsize;

// RECORD and REPLAY FILE
extern char* AudioFilePathandName;
extern char* VideoFilePathandName;

// SGW IP and PORT
extern char* SGW_IP;
extern unsigned short SGW_PORT_Audio;
extern unsigned short SGW_PORT_Video;

// LOCAL IP and PORT
extern char* LOCAL_IP;
extern unsigned short LOCAL_RCV_PORT_Audio;
extern unsigned short LOCAL_RCV_PORT_Video;

extern unsigned short LOCAL_SEND_PORT_Audio;
extern unsigned short LOCAL_SEND_PORT_Video;

// DEST IP and PORT
extern char* DEST_IP;
extern unsigned short DEST_PORT_Audio;
extern unsigned short DEST_PORT_Video;

// DEST AudioIPID and VideoIPID
extern unsigned long DEST_IPID_Audio;
extern unsigned long DEST_IPID_Video;

// LOCAL AudioIPID and VideoIPID
extern unsigned long LOCAL_IPID_Audio;
extern unsigned long LOCAL_IPID_Video;

//extern	char* SendDestIP;					//发送目的地IP
//extern	char* MCASTADDR;					//多播IP地址，若为单播地址，则按单播地址接收
//
//extern	unsigned short AudioDestPORT;		//发送目的地PORT
//extern	unsigned short AudioLocalSendPORT;	//本地发送数据占用的PORT
//extern	unsigned short AudioLocalRcvPORT;	//本地从RCVPORT端口接收数据
//
//extern	unsigned short VideoDestPORT;
//extern	unsigned short VideoLocalSendPORT;
//extern	unsigned short VideoLocalRcvPORT;
//
//extern unsigned long Audio_ipid;
//extern unsigned long Video_ipid;

// ******************************************************************************************* //


// *类对象声明* //
extern  AMR m_AMR;			//AMR类
extern  config m_config;	//多媒体配置类(IP与Port设置，文件路径与文件名设置)


extern BOOL AudioRecordFlag;
extern BOOL VideoRecordFlag;

extern SQTS SQTScontrol;		// 音频给视频的控制信息

extern BOOL SynchronousFlag;

extern BOOL CameraNewOpen;
extern BOOL PLAYRCVONLY;

// *************************************************************************** //
// for open and close audio send
extern BOOL AudioSampleFlag;
extern BOOL AudioEncFlag;
extern BOOL AudioSendFlag;

//extern CEvent EventAudioSampleEnd;
//extern CEvent EventAudioEncEnd;
//extern CEvent EventAudioSendEnd;

// for open and close audio receive
extern BOOL AudioRcvFlag;
extern BOOL AudioDecFlag;
extern BOOL AudioPlayFlag;

//extern CEvent EventAudioRcvEnd;
//extern CEvent EventAudioDecEnd;
//extern CEvent EventAudioPlayEnd;

// for open and close video send
extern BOOL VideoSampleFlag;
extern BOOL VideoEncodFlag;
extern BOOL VideoSendFlag;

extern CEvent EventVideoSampleEnd;
extern CEvent EventVideoEncodEnd;
extern CEvent EventVideoSendEnd;

// for open and close video receive
extern BOOL VideoRcvFlag;
extern BOOL VideoDecFlag;
extern BOOL VideoPlayFlag;

//extern CEvent EventVideoRcvEnd;
//extern CEvent EventVideoDecEnd;
//extern CEvent EventVideoPlayEnd;

// for open and close audio record

extern BOOL AudioRecordFlag;
extern BOOL VideoRecordFlag;

extern CEvent EventAudioRecord;
extern CEvent EventVideoRecord;

//*****************LDS添加
extern CString M4IP1;//LDS添加for showing UDP IP
extern CString M4IP2;
extern CString M4IP3;
extern CString M4IP4;


extern CString M4PORT1;//LDS添加for showing UDP PORT
extern CString M4PORT2;
extern CString M4PORT3;
extern CString M4PORT4;
extern int Acess_Num;//LDS添加为判断哪条等待线路接入
extern Mode My_Mode;
extern int tab_Mode;
extern int H_local;
#endif;