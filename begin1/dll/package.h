#ifndef _package_H_
#define _package_H_

#define AMR_data_Max_Len 32	
#define BUFFSIZE 44//32+12���44
#define RTP_data_Max_Len 50
#define DTPBUFFSIZE 120

#include <stdio.h>
#include <time.h>

#include "GlobalVariable.h"

class package
{
public:
	package(void);
	~package(void);
/*
 * RTP���
 */

	void RTP_Pack_Audio(unsigned  char  AMRFrame[AMR_data_Max_Len],  Virtual_Audio_IP_Package *pVir_A_IP_Package ,short FrameNo);

/*
 * RTP���
 */
	
	
void RTP_Depack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package,unsigned  char AMR_Rec[38]);  


void DtpIpUdp_Pack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package) ;

///*
// * DTP���
// */
//	int DTP_Pack(char RTP_DATA[RTP_data_Max_Len],char sendbuf[DTPBUFFSIZE],int m_RTPFramelength);
///*
// * DTP���
// */
//	void DTP_Depack(char REC_RTP_DATA[RTP_data_Max_Len],char* recbuf);
//
//

/*
 * video DTP���
 */
//void DTP_Depack_Video(const unsigned char* pRecDTPFrame, DepackBuf *pDepackTemp);
	
/*
 * video RTP���
 */

void RTP_Depack_Video(Virtual_Video_IP_Package *pVir_V_IP_Package, Video_H264 *ph264decodTemp);

/*
 * video RTP���
 */

void RTP_Pack_Video(  Video_H264 *ph264frame , Virtual_Video_IP_Package *pVir_V_IP_Package);

void UDP_Pack_Video( Virtual_Video_IP_Package *pVir_V_IP_Package );

void DtpIp_Pack_Video( Virtual_Video_IP_Package *pVir_V_IP_Package, VideoSocketBuf *IP_package_v );

//void DtpIpUdp_Pack_Video( Virtual_Video_IP_Package *pVir_V_IP_Package);



private:


/*
RTP�̶�ͷ�еĸ��ֶεĸ�ʽ: 

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           timestamp                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Synchronization source (SSRC) identifier            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
//X86��little endian��ַ�ṹ���ṹ��λ����ӵ�λ��ʼ����
//RFC 3550�ĵ���big endian��ַ�ṹ
typedef struct{
  	/**//* byte 0 */
	unsigned  char  csrc_len:4;			/*  CSRC  count  */ 
	unsigned  char  extension:1;          /*  header  extension  flag  */  
	unsigned  char  padding:1;            /*  padding  flag  */  
    unsigned  char  version:2;			/*  protocol  version  */  

	/**//* byte 1 */
	unsigned  char  payload:7;				/*  payload  type  */
    unsigned  char  marker :1;				/*  marker  bit  */  
	
	/**//* byte 2��3 */
    unsigned char seq_no_high;
	unsigned char seq_no_low;			/*  sequence  number  */ //Ϊ��little endian��ַ�ṹ�������ߵ��ֽڽ����� 

	/**//* byte 4~7 */                   //          /*  timestamp  */
	unsigned char timestamp_high_high;   //|                timestamp                  |
	unsigned char timestamp_high_low;	// | high_high | high_low | low_high | low_low |
	unsigned char timestamp_low_high;
	unsigned char timestamp_low_low;
	
	/**//* byte 8~11 */
                   						/*  synchronization  source  */  
	unsigned char ssrc_high_high;   // |              synchronization  source      |
	unsigned char ssrc_high_low;	// | high_high | high_low | low_high | low_low |
	unsigned char ssrc_low_high;
	unsigned char ssrc_low_low;
}RTP_FIXED_HEADER;


short temp_seq_no;
long temp_timestamp;
long temp_ssrc;

short	Video_temp_seq_no;
long	Video_temp_timestamp;
long	Video_temp_ssrc;


typedef struct
{
	unsigned long	sourceip;		//ԴIP��ַ
	unsigned long	destip;			//Ŀ��IP��ַ
	unsigned char	mbz;			//�ÿ�(0)
	unsigned char	ptcl;			//Э������
	unsigned short	plen;			//TCP/UDP���ݰ��ĳ���(����TCP/UDP��ͷ�������ݰ������ĳ��� ��λ:�ֽ�)
}Psd_Header;// UDPαͷ


USHORT checksum(unsigned short *buffer,int size)
{
	unsigned long cksum=0;

	while(size>1)
	{
		cksum+=*buffer++;
		size-=sizeof(USHORT);
	}

	if(size)
	{
		cksum+=*(unsigned char *)buffer;
	}
	//��32λ��ת����16
	while (cksum>>16)
		cksum=(cksum>>16)+(cksum & 0xffff);
	
	return (USHORT) (~cksum);
};


};
#endif;