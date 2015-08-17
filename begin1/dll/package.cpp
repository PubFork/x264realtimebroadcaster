#include "stdafx.h"
#include "package.h"

package::package(void)
{
}

package::~package(void)
{
}


void package::RTP_Pack_Audio(unsigned  char  AMRFrame[AMR_data_Max_Len],  Virtual_Audio_IP_Package *pVir_A_IP_Package ,short FrameNo)
{
	srand((unsigned) time(NULL));

	
	
	RTP_FIXED_HEADER rtp_hdr;
	memset( &rtp_hdr, 0, sizeof(rtp_hdr) );
	//设置RTP HEADER参数,不能在结构体定义中赋值
	rtp_hdr.version     = 2;		//版本号，此版本固定为2 //
	rtp_hdr.padding     = 1;		
	rtp_hdr.extension   = 1;
	rtp_hdr.csrc_len    = 0;      /*******计算机内部是按高位到低位存储********/

	rtp_hdr.marker		 = 0;
	rtp_hdr.payload     = 126;	//0011 1111	

	if(FrameNo==0)
	{
	temp_seq_no=rand()%1000;
	temp_timestamp=rand()% 10000;
	temp_ssrc=rand()% 10000;
	}
	temp_seq_no++;
	temp_timestamp+=160;

	rtp_hdr.seq_no_high=(unsigned char)((temp_seq_no&0xff00)>>8);
	rtp_hdr.seq_no_low=(unsigned char)(temp_seq_no&0x00ff);



	rtp_hdr.timestamp_high_high=(unsigned char)((temp_timestamp&0xff000000)>>24);
	rtp_hdr.timestamp_high_low=(unsigned char)((temp_timestamp&0x00ff0000)>>16);
	rtp_hdr.timestamp_low_high=(unsigned char)((temp_timestamp&0x0000ff00)>>8);
	rtp_hdr.timestamp_low_low=(unsigned char)(temp_timestamp&0x000000ff);


	rtp_hdr.ssrc_high_high=(unsigned char)((temp_ssrc&0xff000000)>>24);
	rtp_hdr.ssrc_high_low=(unsigned char)((temp_ssrc&0x00ff0000)>>16);
	rtp_hdr.ssrc_low_high=(unsigned char)((temp_ssrc&0x0000ff00)>>8);
	rtp_hdr.ssrc_low_low=(unsigned char)(temp_ssrc&0x000000ff);
	
	memcpy(pVir_A_IP_Package->RTP_data , ( unsigned char * )&rtp_hdr ,sizeof(rtp_hdr));


	
	unsigned  char  Payload[AMR_data_Max_Len] ={0};  
	int AMR_data_Len,A_P_Len;			//不同速率，每帧不同
	unsigned char FT,FT_b;				// FT值，编码类型，4 bit
								
	unsigned  char GetFT = 120;			//用于从帧头中取得FT值 ，120 =(0-1111-000)
	unsigned  char GetBackTwo = 3;		// 获得后2位， 3 = 0000-0011;
	unsigned  char GetFrontSix = 252;   // 获得前6位， 252 = 1111-1100;
	unsigned  char GetFrontTwo = 192;	// 获得后2位， 192 = 1100-0000;
	unsigned  char GetBackSix = 63;     // 获得前6位， 63 = 0011-1111;
	unsigned char CMR = 15;				// 项目规定：15= (1111) 
	unsigned char Quality = 1;			// 表示未损坏的包
	unsigned  char temp ,i;

	FT= (AMRFrame[0]&GetFT)>>3;			//求得FT的数值
	
	//printf("%d\n",FT);

	//switch(FT)
	//{	
	//	case 0:		// AMR 4.75
	//		AMR_data_Len = 13;
	//		A_P_Len =14;

	//	break;
	//	case 5:		// AMR 7.95
	//		AMR_data_Len = 21;
	//		A_P_Len  = 22;
	//	

	//	break;
	//	case 6:		// AMR 10.2
	//		AMR_data_Len = 27;
	//		A_P_Len = 27;

	//	break;
	//	case 8:		// SID		//??????
	//	break;
	//	case 15:		// NoData		//??
	//	break;
	//	default:
	//		printf("Something wrong in the fist byte!!\n");
	//}
///////////////////////////////////////////////////////////////2012.5.4/////////////////////////////////////////
	switch(FT)
	{	
		case 0:		// AMR 4.75
			AMR_data_Len = 13;
			A_P_Len =14;

		break;

		/////////////////////////////////////////////////////////////////////
        case 1:		// AMR 5.15
			AMR_data_Len = 14;
			A_P_Len =15;

		break;
		case 2:		// AMR 5.90
			AMR_data_Len = 16;
			A_P_Len =17;

		break;
		case 3:		// AMR 6.70
			AMR_data_Len = 18;
			A_P_Len =19;

		break;
		case 4:		// AMR 7.40
			AMR_data_Len = 20;
			A_P_Len =20;

		break;
		/////////////////////////////////////////////////////////////////////
		
		case 5:		// AMR 7.95
			AMR_data_Len = 21;
			A_P_Len  = 22;
		

		break;
		case 6:		// AMR 10.2
			AMR_data_Len = 27;
			A_P_Len = 27;

		break;
		
		///////////////////////////////////////////////////////////////////
        case 7:		// AMR 12.2
			AMR_data_Len = 32;
			A_P_Len =32;

		break;
		///////////////////////////////////////////////////////////////////
		
		case 8:		// SID		//??????
		break;
		case 15:		// NoData		//??
		break;
		default:
			printf("Something wrong in the fist byte!!\n");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	Payload[0] =(Quality<<3)|(FT>>1)|(CMR<<4);
	FT_b =FT<<7;
	Payload[1] =Payload[1]^FT_b;
	Quality = Quality<<6 ;				//Quality是Payload[1]的第2bit
	Payload[1] = Payload[1]^Quality ;	// 此时Payload[1]的第1-2bit赋值结束

	for(i=1;i<AMR_data_Len;i++)
	{
		//AMRFrame[i]的前6位存于Payload[i]的后6位上
		temp = AMRFrame[i] & GetFrontSix; 
		temp = temp>>2;
		Payload[i]=Payload[i]^temp;
		
		//AMRFrame[i]的后2位存于Payload[i+1]的前2位上
		temp = AMRFrame[i] & GetBackTwo;
		temp = temp<<6;
		Payload[i+1] = Payload[i+1]^temp;
	}

	for(i=0;i<A_P_Len;i++)
	{
		pVir_A_IP_Package->RTP_data[12+i]=Payload[i];
	}

	//int RTPFrameLenght = A_P_Len + 12;
	//return RTPFrameLenght;


	pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen		=	A_P_Len  + sizeof(rtp_hdr) + 28 + 8;  
	// 赋值 AMR4.75定长 62字节

}





void package::RTP_Depack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package,unsigned  char AMR_Rec[38])  
{
	int AMR_data_Len,A_P_Len;
    int i;
	unsigned  char temp1,temp2,FT;
	

	memcpy( AMR_Rec,&pVir_A_IP_Package->RTP_data[2] ,6);

	// 完成SN和TS的提取，存于 AMR_Rec的前6字节

	temp1 =pVir_A_IP_Package->RTP_data[12];    
	temp2 =pVir_A_IP_Package->RTP_data[13]; 
	temp1 = temp1&7;
	temp2 = temp2&128;
	FT = (temp1<<1)+(temp2>>7);


	// printf("Depack %d\n",FT);


	//switch(FT)
	//{
	//	case 0:		// AMR 4.75
	//		AMR_data_Len = 13;
	//		A_P_Len =14;
 //           AMR_Rec[6] = 0x04;       //帧头0-0000-100
	//		
	//	break;
	//	case 5:		// AMR 7.95
	//		AMR_data_Len = 21;
	//		A_P_Len  = 22;
	//		AMR_Rec[6] = 0x2c;   //帧头0-0101-100
	//	break;
	//	case 6:		// AMR 10.2
	//		AMR_data_Len = 27;
	//		A_P_Len = 27;
	//		AMR_Rec[6]=0x34;   //帧头0-0110-100
	//	break;
	//	case 8:		// SID		//??????
	//	break;
	//	case 15:		// NoData		//??
	//	break;
	//	default:
	//		printf("Something wrong in the fist byte!!\n"); 
	//}
		/////////////////////////////////////////////////////////////2012.5.4//////////////////////////////////////////
	switch(FT)
	{
		case 0:		// AMR 4.75
			AMR_data_Len = 13;
			A_P_Len =14;
            AMR_Rec[6] = 0x04;       //帧头0-0000-100
			
		break;
		
		/////////////////////////////////////////////////////////////////////
        case 1:		// AMR 5.15
			AMR_data_Len = 14;
			A_P_Len =15;
			AMR_Rec[6] = 0x0c;

		break;
		case 2:		// AMR 5.90
			AMR_data_Len = 16;
			A_P_Len =17;
			AMR_Rec[6] = 0x14;

		break;
		case 3:		// AMR 6.70
			AMR_data_Len = 18;
			A_P_Len =19;
			AMR_Rec[6] = 0x1c;

		break;
		case 4:		// AMR 7.40
			AMR_data_Len = 20;
			A_P_Len =20;
			AMR_Rec[6] = 0x24;

		break;
		/////////////////////////////////////////////////////////////////////
		
		case 5:		// AMR 7.95
			AMR_data_Len = 21;
			A_P_Len  = 22;
			AMR_Rec[6] = 0x2c;   //帧头0-0101-100
		break;
		case 6:		// AMR 10.2
			AMR_data_Len = 27;
			A_P_Len = 27;
			AMR_Rec[6]=0x34;   //帧头0-0110-100
		break;
		
		///////////////////////////////////////////////////////////////////
        case 7:		// AMR 12.2
			AMR_data_Len = 32;
			A_P_Len =32;
			AMR_Rec[6] = 0x3c;

		break;
		///////////////////////////////////////////////////////////////////
		
		case 8:		// SID		//??????
		break;
		case 15:		// NoData		//??
		break;
		default:
			printf("Something wrong in the fist byte!!\n"); 
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned  char  Payload[32] ={0}; 
	for(i=1;i<=A_P_Len-1;i++)
	{
	  Payload[i]=pVir_A_IP_Package->RTP_data[12+i];
	}
	unsigned  char GetFrontTwo = 192;	// 获得后2位， 192 = 1100-0000;
	unsigned  char GetBackSix = 63;     // 获得前6位， 63 = 0011-1111;
	unsigned  char temp_a,temp_b;

	for(i=1;i<AMR_data_Len;i++)
	{
		temp_a = Payload[i] & GetBackSix;
		temp_a= temp_a<<2;
		temp_b= Payload[i+1]&GetFrontTwo;
		temp_b= temp_b>>6;
		AMR_Rec[6+i] = temp_a|temp_b;

	}	
}

	void package::DtpIpUdp_Pack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package) 
	// 	void package::DtpIpUdp_Pack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package, 发送buff)  ??? 
{
	// DTP_Pack
	pVir_A_IP_Package->mDtpuhdr.bDtpUFirstOct = 0x28;
	pVir_A_IP_Package->mDtpuhdr.bDtpUMsgType  = 15 ; // 暂定值 ！！！
	pVir_A_IP_Package->mDtpuhdr.dwDtpUIpId    = 0  ;
	// pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen 已经在RTP_Pack完成，值为62



	// IP_Pack

	pVir_A_IP_Package->mIphdr.ip_verlen =	(4<<4 |(sizeof(IPV4_HDR)/sizeof(ULONG)));;
	pVir_A_IP_Package->mIphdr.ip_tos	=	0;
	pVir_A_IP_Package->mIphdr.ip_totallength	= pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen - 8;	//htons(sizeof(IPV4_HDR) + sizeof(UDP_HDR) + msglen);
	pVir_A_IP_Package->mIphdr.ip_id		=	0 ;
	pVir_A_IP_Package->mIphdr.ip_offset =	0 ;
	pVir_A_IP_Package->mIphdr.ip_ttl	=	64;
	pVir_A_IP_Package->mIphdr.ip_protocol	=	IPPROTO_UDP;
	pVir_A_IP_Package->mIphdr.ip_checksum	=	0;
	pVir_A_IP_Package->mIphdr.ip_srcaddr	=	0; // inet_addr( char szSourceIp[]	= "192.168.11.223");
	pVir_A_IP_Package->mIphdr.ip_destaddr	=	0; // inet_addr(char szDestIp[]		= "192.168.11.233");

	pVir_A_IP_Package->mIphdr.ip_checksum   =	checksum((USHORT *)&pVir_A_IP_Package->mIphdr,sizeof(IPV4_HDR));

	// UDP_Pack
	unsigned char udpbuf[1024];

	Psd_Header ppsdhdr;

	
	memset(&ppsdhdr, 0, sizeof(ppsdhdr));

	ppsdhdr.sourceip	=	0;	// inet_addr(szSourceIp);
	ppsdhdr.destip		=	0;  //inet_addr(szDestIp);
	ppsdhdr.mbz		=	0	;
	ppsdhdr.ptcl	= IPPROTO_UDP;
	ppsdhdr.plen	=	pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen-28;	// htons(sizeof(UDP_HDR) + msglen)
	


	pVir_A_IP_Package->mUdphdr.src_portno	=	0;//htons(sourcePort);
	pVir_A_IP_Package->mUdphdr.dst_portno	=	0;//htons(destPort);
	pVir_A_IP_Package->mUdphdr.udp_length	=	pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen-28;
	pVir_A_IP_Package->mUdphdr.udp_checksum	=	0;





	
	memcpy(udpbuf, (char*)&ppsdhdr, sizeof(ppsdhdr));
	memcpy(&udpbuf[sizeof(ppsdhdr)], (char*)&pVir_A_IP_Package->mUdphdr, sizeof(pVir_A_IP_Package->mUdphdr));
	
	
	
	memcpy(&udpbuf[sizeof(ppsdhdr) + sizeof(pVir_A_IP_Package->mUdphdr)], pVir_A_IP_Package->RTP_data, pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen - 36);
	
	int udpbuflen = sizeof(ppsdhdr) + sizeof(pVir_A_IP_Package->mUdphdr) + pVir_A_IP_Package->mDtpuhdr.wDtpUMsgLen - 36;

	
	pVir_A_IP_Package->mUdphdr.udp_checksum	=	checksum((unsigned short *)udpbuf,udpbuflen);
}








//  视频RTP拆包
void package::RTP_Depack_Video(Virtual_Video_IP_Package *pVir_V_IP_Package, Video_H264 *ph264decodTemp)
{
	RTP_FIXED_HEADER pRtp_hdr;

	//char p_nal[4];
	//p_nal[0] = p_nal[1] = p_nal[2] = 0;
	//p_nal[3] = 1;

	memset(&pRtp_hdr,0,sizeof(RTP_FIXED_HEADER));

	memcpy(&pRtp_hdr, pVir_V_IP_Package->RTP_data, 12);

	ph264decodTemp->slicelen = ntohs( pVir_V_IP_Package->mUdphdr.udp_length ) - 8 - 12;

	ph264decodTemp->sq = ((short)pRtp_hdr.seq_no_high<<8)^((short)pRtp_hdr.seq_no_low);
	
	ph264decodTemp->ts = ((long)pRtp_hdr.timestamp_high_high <<24)^((long)pRtp_hdr.timestamp_high_low<<16)^((long)pRtp_hdr.timestamp_low_high<<8)^((long)pRtp_hdr.timestamp_low_low);

	//memcpy(ph264decodTemp->slicebuf, p_nal, 4);
	memcpy(ph264decodTemp->slicebuf, &pVir_V_IP_Package->RTP_data[12], ph264decodTemp->slicelen);

}


void package::RTP_Pack_Video( Video_H264 *ph264frame, Virtual_Video_IP_Package *pVir_V_IP_Package )
{
	srand((unsigned) time(NULL));
	RTP_FIXED_HEADER rtp_hdr;
	memset( &rtp_hdr, 0, sizeof(rtp_hdr) );
	//rtp固定包头，为12字节,该句将RTP_data[0]的地址赋给rtp_hdr，以后对rtp_hdr的写入操作将直接写入RTP_data。
	//设置RTP HEADER参数,不能在结构体定义中赋值
	rtp_hdr.version     = 2;		//版本号，此版本固定为2 //
	rtp_hdr.padding     = 0;		
	rtp_hdr.extension   = 0;
	rtp_hdr.csrc_len	= 0;      /*******计算机内部是按高位到低位存储********/

	rtp_hdr.marker		= 0;
	rtp_hdr.payload		= 127;		

	if( ph264frame->sq == 0 )
	{
		srand((unsigned long)time(0));
		Video_temp_seq_no = rand()%1000;
		Video_temp_timestamp = rand()%1000;
		Video_temp_ssrc = rand()% 2000 + 16779215;
	}
	Video_temp_seq_no ++;
	Video_temp_timestamp += 3600;

	rtp_hdr.seq_no_high	= (unsigned char)((Video_temp_seq_no&0xff00)>>8);
	rtp_hdr.seq_no_low	= (unsigned char)(Video_temp_seq_no&0x00ff);

	rtp_hdr.timestamp_high_high	= (unsigned char)((Video_temp_timestamp&0xff000000)>>24);
	rtp_hdr.timestamp_high_low	= (unsigned char)((Video_temp_timestamp&0x00ff0000)>>16);
	rtp_hdr.timestamp_low_high	= (unsigned char)((Video_temp_timestamp&0x0000ff00)>>8);
	rtp_hdr.timestamp_low_low	= (unsigned char)(Video_temp_timestamp&0x000000ff);

	rtp_hdr.ssrc_high_high	= (unsigned char)((Video_temp_ssrc&0xff000000)>>24);
	rtp_hdr.ssrc_high_low	= (unsigned char)((Video_temp_ssrc&0x00ff0000)>>16);
	rtp_hdr.ssrc_low_high	= (unsigned char)((Video_temp_ssrc&0x0000ff00)>>8);
	rtp_hdr.ssrc_low_low	= (unsigned char)(Video_temp_ssrc&0x000000ff);
																																			   
	memcpy(&pVir_V_IP_Package->RTP_data, ( unsigned char * )&rtp_hdr, sizeof(rtp_hdr));					  // RTP头 + 264信息 ----RTP_data
	memcpy(&pVir_V_IP_Package->RTP_data[sizeof(rtp_hdr)], ph264frame->slicebuf, ph264frame->slicelen);
	pVir_V_IP_Package->data_len = ph264frame->slicelen + 12;			   // data_len =  RTP头 + 264信息 ----RTP_data


	//pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen	= ph264frame->slicelen + sizeof(rtp_hdr) + 28 - 4;
	//pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen	= ph264frame->slicelen + sizeof(rtp_hdr) + 28;
}

void package::UDP_Pack_Video(Virtual_Video_IP_Package *pVir_V_IP_Package)
{
	unsigned char udpbuf[MAX_VIDEO_LEN];

	Psd_Header ppsdhdr;
	memset(udpbuf, 0, MAX_VIDEO_LEN);
	memset(&ppsdhdr, 0, sizeof(ppsdhdr));

	ppsdhdr.sourceip	= inet_addr(LOCAL_IP);	// inet_addr(szSourceIp);
	ppsdhdr.destip		= inet_addr(DEST_IP);	//inet_addr(szDestIp);
	ppsdhdr.mbz			= 0;
	ppsdhdr.ptcl		= IPPROTO_UDP;
	ppsdhdr.plen		= htons( 8 + pVir_V_IP_Package->data_len );	// htons(sizeof(UDP_HDR) + msglen)

	pVir_V_IP_Package->mUdphdr.src_portno	= htons( LOCAL_SEND_PORT_Video );		//htons(sourcePort);
	pVir_V_IP_Package->mUdphdr.dst_portno	= htons( DEST_PORT_Video );				//htons(destPort);
	pVir_V_IP_Package->mUdphdr.udp_length	= htons( 8 + pVir_V_IP_Package->data_len );	// htons(sizeof(UDP_HDR) + msglen)
	pVir_V_IP_Package->mUdphdr.udp_checksum	= 0;

	
	
	memcpy(udpbuf, (char*)&ppsdhdr, sizeof(ppsdhdr));
	memcpy(&udpbuf[sizeof(ppsdhdr)], (char*)&pVir_V_IP_Package->mUdphdr, sizeof(pVir_V_IP_Package->mUdphdr));

	memcpy(&udpbuf[sizeof(ppsdhdr) + sizeof(pVir_V_IP_Package->mUdphdr)], pVir_V_IP_Package->RTP_data, pVir_V_IP_Package->data_len);

	int udpbuflen = sizeof(ppsdhdr) + sizeof(pVir_V_IP_Package->mUdphdr) + pVir_V_IP_Package->data_len;  //data_len =  RTP头 + 264信息 ----RTP_data




	pVir_V_IP_Package->mUdphdr.udp_checksum	= checksum((unsigned short *)udpbuf,udpbuflen);				// psd伪头12字节 


	//pVir_V_IP_Package->data_len += 8;						why?? 没放到data里
}


void package::DtpIp_Pack_Video( Virtual_Video_IP_Package *pVir_V_IP_Package, VideoSocketBuf *IP_package_v )
{
	unsigned short	ip_RF;
	unsigned short	ip_DF;
	unsigned short	ip_MF;
	unsigned short	ip_OFFMASK;

	if ( pVir_V_IP_Package->mIphdr.ip_verlen == 0 )		// NO.1 ip_package
	{
		//printf("case1 ip_verlen == 0");
		
		pVir_V_IP_Package->mIphdr.ip_verlen			= (4<<4 |(sizeof(IPV4_HDR)/sizeof(ULONG)));
		pVir_V_IP_Package->mIphdr.ip_tos			= 0;		
		pVir_V_IP_Package->mIphdr.ip_id				= (USHORT)GetCurrentProcessId();
		pVir_V_IP_Package->mIphdr.ip_ttl			= 64;
		pVir_V_IP_Package->mIphdr.ip_protocol		= IPPROTO_UDP;
		pVir_V_IP_Package->mIphdr.ip_checksum		= 0;
		pVir_V_IP_Package->mIphdr.ip_srcaddr		= inet_addr(LOCAL_IP);
		pVir_V_IP_Package->mIphdr.ip_destaddr		= inet_addr(DEST_IP);

		if ( ( pVir_V_IP_Package->data_len + 8 + 20 ) > IP_IN_PACKAGE_MTU )
		{
			pVir_V_IP_Package->mIphdr.ip_totallength	= htons( IP_IN_PACKAGE_MTU );
			ip_RF = 0;
			ip_DF = 0;
			ip_MF = 0x2000;		// 0010 0000 0000 0000
			ip_OFFMASK = 0;
			pVir_V_IP_Package->mIphdr.ip_offset			= htons( ip_RF + ip_DF + ip_MF + ip_OFFMASK );
		}



		else
		{
			pVir_V_IP_Package->mIphdr.ip_totallength	= htons( 20 + 8 + pVir_V_IP_Package->data_len );	//htons(sizeof(IPV4_HDR) + sizeof(UDP_HDR) + msglen);::mIphdr.ip_totallength
			pVir_V_IP_Package->mIphdr.ip_offset			= 0;
		}

		pVir_V_IP_Package->mIphdr.ip_checksum		= checksum((USHORT *)&pVir_V_IP_Package->mIphdr,sizeof(IPV4_HDR));


		pVir_V_IP_Package->mDtpuhdr.bDtpUFirstOct	= 0x28;
		pVir_V_IP_Package->mDtpuhdr.bDtpUMsgType	= 255; // 暂定值 ！！！
		pVir_V_IP_Package->mDtpuhdr.dwDtpUIpId		= DEST_IPID_Video;

		
		
		if ( ntohs( pVir_V_IP_Package->mIphdr.ip_totallength ) < IP_IN_PACKAGE_MIN )
			pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen = htons( IP_IN_PACKAGE_MIN );
		    
		else
			pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen = pVir_V_IP_Package->mIphdr.ip_totallength;    //mIphdr.ip_totallength=+20+8+data_len ( RTP头 + 264信息 ----RTP_data)
		    printf("大于IPmin");
		IP_package_v->datalen = ntohs( pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen ) + 8 ;  //+8 是DTP头？




		memcpy( IP_package_v->SocketData, (unsigned char*)pVir_V_IP_Package, IP_package_v->datalen );		 //从结构体复制到SocketData？？？



	//	printf("V_IP_Package的DTP头，IP头 UDP头 RTPdata， 数据长度datalen： %X\n  %X\n  %X\n   %X\n ",pVir_V_IP_Package->mDtpuhdr, pVir_V_IP_Package->mIphdr,pVir_V_IP_Package->mUdphdr,pVir_V_IP_Package->RTP_data,pVir_V_IP_Package->data_len);
		
		
		
		pVir_V_IP_Package->data_len -= ( ntohs(pVir_V_IP_Package->mIphdr.ip_totallength) - 20 - 8 );	   // -UDP-IP头
         
	//	printf("IP_package_v->datalen =====%d\n     IP_package_v->SocketData=====%d \n",IP_package_v->datalen, IP_package_v->SocketData );

		 printf("data_len 传递正常");
	}







	else
	{
		printf("case2 ip_verlen !== 0");
		ip_OFFMASK = ntohs( pVir_V_IP_Package->mIphdr.ip_offset ) - 0x2000;
		pVir_V_IP_Package->mIphdr.ip_checksum = 0;

		if ( pVir_V_IP_Package->data_len + 20 > IP_IN_PACKAGE_MTU )
		{
			pVir_V_IP_Package->mIphdr.ip_totallength = htons( IP_IN_PACKAGE_MTU );
			ip_RF = 0;
			ip_DF = 0;
			ip_MF = 0x2000;		// 0010 0000 0000 0000
		}
		else
		{
			pVir_V_IP_Package->mIphdr.ip_totallength = htons( 20 + pVir_V_IP_Package->data_len );	//htons(sizeof(IPV4_HDR) + sizeof(UDP_HDR) + msglen);
			ip_RF = 0;
			ip_DF = 0;
			ip_MF = 0;		// 0000 0000 0000 0000
		}

		ip_OFFMASK += ( ( IP_IN_PACKAGE_MTU - 20) / 8 );
		pVir_V_IP_Package->mIphdr.ip_offset			= htons( ip_RF + ip_DF + ip_MF + ip_OFFMASK );

		pVir_V_IP_Package->mIphdr.ip_checksum		= checksum((USHORT *)&pVir_V_IP_Package->mIphdr,sizeof(IPV4_HDR));

		if ( ntohs( pVir_V_IP_Package->mIphdr.ip_totallength ) < IP_IN_PACKAGE_MIN )
			pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen = htons( IP_IN_PACKAGE_MIN );

		else
			pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen = pVir_V_IP_Package->mIphdr.ip_totallength;

		//printf("判定完毕 继续进行");
																				             //	mIphdr.ip_totallength为UDP+IP+RTPdata长度    ==  mDtpuhdr.wDtpUMsgLen
		IP_package_v->datalen = ntohs( pVir_V_IP_Package->mDtpuhdr.wDtpUMsgLen ) + 8;		 //	 即 pVir_V_IP_Package->mIphdr.ip_totallength + 8

		memcpy(&IP_package_v->SocketData, (unsigned char*)&pVir_V_IP_Package->mDtpuhdr, 8);		  //先放DTPhdr 再放IPhdr 最后RTPdata
		
		memcpy(&IP_package_v->SocketData[8], (unsigned char*)&pVir_V_IP_Package->mIphdr, 20);

		memcpy(&IP_package_v->SocketData[8 + 20], &pVir_V_IP_Package->RTP_data[ip_OFFMASK * 8 - 8], IP_package_v->datalen - 8 - 20);

		// UDP没搞懂 
	//	printf("IPpackage SocketData=:=  %d\n,  IP_package_v->datalen=:= %d\n",&IP_package_v->SocketData,IP_package_v->datalen);





		pVir_V_IP_Package->data_len -= ( ntohs( pVir_V_IP_Package->mIphdr.ip_totallength ) - 20 );
	}
}


//void package::DtpIpUdp_Pack_Video( Virtual_Video_IP_Package *pVir_V_IP_Package ) 
//	// 	void package::DtpIpUdp_Pack_Audio(Virtual_Audio_IP_Package *pVir_A_IP_Package, 发送buff)  ??? 
//{
//	DTP_U_HEADER *dtp_hdr=(DTP_U_HEADER*)sendbuf;
//	dtp_hdr->bDtpUFirstOct = 0x28;
//	dtp_hdr->bDtpUMsgType = 15 ; // 暂定值Message Type
//	dtp_hdr->wDtpUMsgLen = m_RTPFramelength + 8;
//	dtp_hdr->dwDtpUIpId = Audio_ipid;
//
//	int i;
//	for(i=0;i<RTP_data_Max_Len;i++)
//        sendbuf[8+i]=RTP_DATA[i];
//	int DTPFrameLenght=m_RTPFramelength+8;
//	return DTPFrameLenght;
//}
////  AMR 的DTP头没有按照2.0项目改动。需要重新改！！
////  AMR 没有进行DTP的打包和拆包！！！
//void package::DTP_Depack(char REC_RTP_DATA[RTP_data_Max_Len],char* recbuf)
//{
//	for(int i=0;i<RTP_data_Max_Len;i++) REC_RTP_DATA[i]=*(recbuf+i+8);
//}
//


////  视频DTP拆包
//void package::DTP_Depack_Video(const unsigned char *pRecDTPFrame, DepackBuf *pDepackTemp)
//{   
//	DTP_U_HEADER pDtp_hdr;
//
//	memset(&pDtp_hdr, 0, sizeof(DTP_U_HEADER));
//
//	memcpy(&pDtp_hdr, pRecDTPFrame, 8);
//
//	pDepackTemp->bDtpUFirstOct	= pDtp_hdr.bDtpUFirstOct;
//	pDepackTemp->bDtpUMsgType	= pDtp_hdr.bDtpUMsgType;
//	pDepackTemp->dwDtpUIpId		= pDtp_hdr.dwDtpUIpId;
//	pDepackTemp->wDtpUMsgLen	= pDtp_hdr.wDtpUMsgLen;
//
//	memcpy(pDepackTemp->RTP_data, &pRecDTPFrame[8], pDepackTemp->wDtpUMsgLen);
//}
//





//
//
//
////  H.264 DTP打包
//void package::DTP_Pack_Video( VideoSocketBuf *pSocketDTPframe, DepackBuf  *Struct_RTP_data) 
//{
//	Struct_RTP_data->bDtpUFirstOct = 0x28;
//	Struct_RTP_data->bDtpUMsgType  = 0xff;
//	Struct_RTP_data->dwDtpUIpId    = Video_ipid;
//
//	memcpy( pSocketDTPframe->localbuf, (unsigned char*)Struct_RTP_data, Struct_RTP_data->wDtpUMsgLen + 8 );
//	pSocketDTPframe->slicelen = Struct_RTP_data->wDtpUMsgLen + 8;
//}
//
