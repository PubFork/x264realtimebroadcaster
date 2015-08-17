#ifndef _config_H_
#define _config_H_

//#include "GlobalVariable.h"

class config
{
public:
	config(void);
	~config(void);

// ************************************************************************************* //
//// Video Display for Delphi
//DWORD HANDLE_DISPLAY;
//int coordinate_x;
//int coordinate_y;
//int wsize;
//int hsize;
	void Video_displayer_set(unsigned long HANDLE_DISPLAY_D, int coordinate_x_D, int coordinate_y_D, int wsize_D, int hsize_D);
	void Video_displayer_setA(unsigned long AHANDLE_DISPLAY_D, int Acoordinate_x_D, int Acoordinate_y_D, int Awsize_D, int Ahsize_D);

//// RECORD and REPLAY FILE
//char* AudioFilePathandName;
//char* VideoFilePathandName;
	void SetAudioFilePathandName(char* AudioFilePathandName_SET);
	void SetVideoFilePathandName(char* VideoFilePathandName_SET);

//// SGW IP and PORT
//char* SGW_IP;
//unsigned short SGW_PORT_Audio;
//unsigned short SGW_PORT_Video;
void SetSGWIPandPORT(char *SGW_IP_SET,
							 unsigned short SGW_PORT_Audio_SET,unsigned short SGW_PORT_Video_SET);

//// LOCAL IP and PORT
//char* LOCAL_IP;
//unsigned short LOCAL_RCV_PORT_Audio;
//unsigned short LOCAL_RCV_PORT_Video;
//
//unsigned short LOCAL_SEND_PORT_Audio;
//unsigned short LOCAL_SEND_PORT_Video;
void SetLOCALIPandPORT(char *LOCAL_IP_SET,
							   unsigned short LOCAL_RCV_PORT_Audio_SET,
							   unsigned short LOCAL_SEND_PORT_Audio_SET, unsigned short LOCAL_RCV_PORT_Video_SET,
							   unsigned short LOCAL_SEND_PORT_Video_SET);

//// DEST IP and PORT
//char* DEST_IP;
//unsigned short DEST_PORT_Audio;
//unsigned short DEST_PORT_Video;
void config::SetDESTIPandPORT(char *DEST_IP_SET,
							  unsigned short DEST_PORT_Audio_SET,unsigned short DEST_PORT_Video_SET);

//// DEST AudioIPID and VideoIPID
//unsigned long DEST_IPID_Audio;
//unsigned long DEST_IPID_Video;
	void SetDESTIPID(unsigned long DEST_IPID_Audio_SET,unsigned long DEST_IPID_Video_SET);

//// LOCAL AudioIPID and VideoIPID
//unsigned long LOCAL_IPID_Audio;
//unsigned long LOCAL_IPID_Video;
	void SetLOCALIPID(unsigned long LOCAL_IPID_Audio_SET,unsigned long LOCAL_IPID_Video_SET);

// ************************************************************************************* //


};

#endif;