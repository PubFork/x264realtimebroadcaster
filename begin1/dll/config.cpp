#include "stdafx.h"
#include "config.h"
#include "GlobalVariable.h"
config::config(void)
{
}

config::~config(void)
{
}
/*函数功能：收发IP与port配置
形参：IP_SEND 发送目的IP
	  PORT_SEND 发送目的端口
	  PORT_RECEIVE 本机接收端口

调用举例：
     SetIPandPort("192.168.0.18",62000,61000);
*/

//// Video Display for Delphi
//DWORD HANDLE_DISPLAY;
//int coordinate_x;
//int coordinate_y;
//int wsize;
//int hsize;
void config::Video_displayer_set(unsigned long HANDLE_DISPLAY_D, int coordinate_x_D, int coordinate_y_D, int wsize_D, int hsize_D)
{
	HANDLE_DISPLAY = HANDLE_DISPLAY_D;
	coordinate_x = coordinate_x_D;
	coordinate_y = coordinate_y_D;
	wsize = wsize_D;
	hsize = hsize_D;
}

void config::Video_displayer_setA(unsigned long AHANDLE_DISPLAY_D, int Acoordinate_x_D, int Acoordinate_y_D, int Awsize_D, int Ahsize_D)
{
	AHANDLE_DISPLAY = AHANDLE_DISPLAY_D;
	Acoordinate_x = Acoordinate_x_D;
	Acoordinate_y = Acoordinate_y_D;
	Awsize = Awsize_D;
	Ahsize = Ahsize_D;
}


//// RECORD and REPLAY FILE
//char* AudioFilePathandName;
//char* VideoFilePathandName;
void config::SetAudioFilePathandName(char* AudioFilePathandName_SET)
{
	AudioFilePathandName = AudioFilePathandName_SET;
}

void config::SetVideoFilePathandName(char *VideoFilePathandName_SET)
{
	VideoFilePathandName = VideoFilePathandName_SET;
}

//// SGW IP and PORT
//char* SGW_IP;
//unsigned short SGW_PORT_Audio;
//unsigned short SGW_PORT_Video;
void config::SetSGWIPandPORT(char *SGW_IP_SET,
							 unsigned short SGW_PORT_Audio_SET,unsigned short SGW_PORT_Video_SET)
{
	SGW_IP = SGW_IP_SET;
	SGW_PORT_Audio = SGW_PORT_Audio_SET;
	SGW_PORT_Video = SGW_PORT_Video_SET;
}

//// LOCAL IP and PORT
//char* LOCAL_IP;
//unsigned short LOCAL_RCV_PORT_Audio;
//unsigned short LOCAL_RCV_PORT_Video;
//
//unsigned short LOCAL_SEND_PORT_Audio;
//unsigned short LOCAL_SEND_PORT_Video;
void config::SetLOCALIPandPORT(char *LOCAL_IP_SET,
							   unsigned short LOCAL_RCV_PORT_Audio_SET,
							   unsigned short LOCAL_SEND_PORT_Audio_SET, unsigned short LOCAL_RCV_PORT_Video_SET,
							   unsigned short LOCAL_SEND_PORT_Video_SET)
{
	LOCAL_IP = LOCAL_IP_SET;
	LOCAL_RCV_PORT_Audio = LOCAL_RCV_PORT_Audio_SET;
	LOCAL_SEND_PORT_Audio = LOCAL_SEND_PORT_Audio_SET;
	LOCAL_RCV_PORT_Video = LOCAL_RCV_PORT_Video_SET;
	LOCAL_SEND_PORT_Video = LOCAL_SEND_PORT_Video_SET;
	
}

//// DEST IP and PORT
//char* DEST_IP;
//unsigned short DEST_PORT_Audio;
//unsigned short DEST_PORT_Video;
void config::SetDESTIPandPORT(char *DEST_IP_SET,
							  unsigned short DEST_PORT_Audio_SET,unsigned short DEST_PORT_Video_SET)
{
	DEST_IP = DEST_IP_SET;
	DEST_PORT_Audio = DEST_PORT_Audio_SET;
	DEST_PORT_Video = DEST_PORT_Video_SET;
}

//// DEST AudioIPID and VideoIPID
//unsigned long DEST_IPID_Audio;
//unsigned long DEST_IPID_Video;
void config::SetDESTIPID(unsigned long DEST_IPID_Audio_SET,unsigned long DEST_IPID_Video_SET)
{
	DEST_IPID_Audio = DEST_IPID_Audio_SET;
	DEST_IPID_Video = DEST_IPID_Video_SET;
}

//// LOCAL AudioIPID and VideoIPID
//unsigned long LOCAL_IPID_Audio;
//unsigned long LOCAL_IPID_Video;
void config::SetLOCALIPID(unsigned long LOCAL_IPID_Audio_SET,unsigned long LOCAL_IPID_Video_SET)
{
	LOCAL_IPID_Audio = LOCAL_IPID_Audio_SET;
	LOCAL_IPID_Video = LOCAL_IPID_Video_SET;
}


