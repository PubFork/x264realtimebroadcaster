#include "stdafx.h"
#include "SocketThread.h"


package m_package;
package a_enc_package;
package a_dec_package;



// ********************************************************************************************** //
// Audio UDP Recevie Thread
// ********************************************************************************************** //
unsigned WINAPI AudioUDPSocketReceiveThread(void* pvParam)
{
	int Apnum = 0;
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 0 );
	WSAStartup( wVersionRequested,&wsaData );   //��winsocket��ʼ��

	SOCKET ReceiveSocket;									//�����׽���
	ReceiveSocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);	//���ݱ���UDP

	int iMode = 0;											//0��������1��������
	ioctlsocket(ReceiveSocket,FIONBIO,(u_long FAR*)&iMode);	//����Ϊ����ģʽ

	sockaddr_in   LocalAddr;
	sockaddr_in   RemoteAddr;
	//ip_mreq		  mcast;								//�ಥ��ַ�ṹ��
	int len = sizeof(SOCKADDR);

	LocalAddr.sin_family = AF_INET;										//Internet
	LocalAddr.sin_port = htons(LOCAL_RCV_PORT_Audio);					//����PORT   
	LocalAddr.sin_addr.s_addr = INADDR_ANY;								//����IP 
	bind(ReceiveSocket,(SOCKADDR*)&LocalAddr,len);						//�󶨱���IP�Ͷ˿�

	//mcast.imr_multiaddr.S_un.S_addr = inet_addr(MCASTADDR);										//�ಥIP����ΪҪ����Ķಥ��
	//mcast.imr_interface.S_un.S_addr = htonl(INADDR_ANY);										//�ӿ�Ϊ����
	//setsockopt(ReceiveSocket,IPPROTO_IP,IP_ADD_MEMBERSHIP,(CHAR*)&mcast,sizeof(mcast));			//����IP��ಥIP����

	char RecvBuf[255];
	AudioSocketBuf InBuf;
	AMROutBuf AMRSockeBuf;
	Virtual_Audio_IP_Package mVirtual_Audio_IP_PackageR;
	unsigned char AMR_Rec[38];

	memset( &InBuf, 0, sizeof(InBuf) );
	memset( &AMRSockeBuf, 0, sizeof(AMRSockeBuf) );
	printf("Audio Socket Receiving\n");

	//****Ϊ���������ڲ�ͬ�û������ݰ���׼��
	CString addr;//for IP
	CString addp;//for Port
	CString str;
	int flag;
		 fd_set   fdsetRead; 
        timeval   tv; 
        tv.tv_sec=3;   //��ʱֵ3�� 
        tv.tv_usec=3000;//3000ms 
        int   nResult;    //��ʱ��־
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{if (msg.message==WM_TERM_THREAD) 
		{closesocket(ReceiveSocket) ;                        //�ر�
		WSACleanup();
		return 0;}
		}
//********************************************************************************************** //
				FD_ZERO(&fdsetRead); //********���������ȴ�ʱ��
                FD_SET(ReceiveSocket,&fdsetRead); 
                nResult=select(0,&fdsetRead,NULL,NULL,&tv); 
                if(nResult==1) 
                { 
		int recvlen = recvfrom(ReceiveSocket, RecvBuf, sizeof(RecvBuf), 0, (SOCKADDR*)&RemoteAddr,&len);
		if( recvlen != SOCKET_ERROR)
			//����RecvBuf��С�����ݰ��ᱻ������
		{	flag=0;//;���������ݰ���û�б�����
		Apnum++;
		printf("Audio Receive%d\n",Apnum);
		for(int i=0;i<sizeof(RecvBuf);i++)
		{
			InBuf.SocketData[i] = (unsigned char)RecvBuf[i];
		}

		//memcpy( InBuf.SocketData, RecvBuf, recvlen );
		//memcpy( InBuf.SocketData, (unsigned char*)RecvBuf, recvlen );
		InBuf.datalen = recvlen;

		// ********************************************************************************************** //



		memcpy(&mVirtual_Audio_IP_PackageR , InBuf.SocketData , InBuf.datalen);			

		a_dec_package.RTP_Depack_Audio( &mVirtual_Audio_IP_PackageR, AMR_Rec );

		AMRSockeBuf.sequencenum = ((short)AMR_Rec[0]<<8)^((short)AMR_Rec[1]);
		AMRSockeBuf.timestamp   = ((long)AMR_Rec[2]<<24)^((long)AMR_Rec[3]<<16)^((long)AMR_Rec[4]<<8)^((long)AMR_Rec[5]);

		memcpy(AMRSockeBuf.AMR_analysis, &AMR_Rec[6], 32);


		// ********************************************************************************************** //

		//**************************************************************LDS���
		addr.Empty(); 
		addp.Empty(); 
		str.Format(_T("%u"),RemoteAddr.sin_addr.S_un.S_un_b.s_b1);
		addr+=str;
		addr+=".";
		str.Format(_T("%u"),RemoteAddr.sin_addr.S_un.S_un_b.s_b2);
		addr+=str;
		addr+=".";
		str.Format(_T("%u"),RemoteAddr.sin_addr.S_un.S_un_b.s_b3);
		addr+=str;
		addr+=".";
		str.Format(_T("%u"),RemoteAddr.sin_addr.S_un.S_un_b.s_b4);
		addr+=str;
		str.Format(_T("%u"),ntohs(RemoteAddr.sin_port));
		addp=str;

		if(flag==0)
		{if( M4IP1==addr&&M4PORT1==addp)
		{if(Acess_Num==1)
		{critical_section3_1.Lock();
		pRecvfromBufferQueue_1.push(AMRSockeBuf);
		critical_section3_1.Unlock();
		}flag=1;}
		else if( M4IP2==addr&&M4PORT2==addp)
		{if(Acess_Num==2)
		{critical_section3_1.Lock();
		pRecvfromBufferQueue_1.push(AMRSockeBuf);
		critical_section3_1.Unlock();
		}flag=1;}
		}


		if(flag==0)
		{
			if(M4IP1.IsEmpty()!=0)
			{M4IP1=addr;
			M4PORT1=addp;
			if (Acess_Num==1){
				critical_section3_1.Lock();
				pRecvfromBufferQueue_1.push(AMRSockeBuf);
				critical_section3_1.Unlock();
			}flag=1;}
			else if(M4IP2.IsEmpty()!=0)
			{M4IP2=addr;
			M4PORT2=addp;
			if( Acess_Num==2){
				critical_section3_1.Lock();
				pRecvfromBufferQueue_1.push(AMRSockeBuf);
				critical_section3_1.Unlock();
			}flag=1;}
		}


		}

		}
		Sleep(1);
	}

	closesocket(ReceiveSocket);
	WSACleanup();

	printf("Thread OVER : AudioUDPSocketReceiveThread\n");

	return 0;
}

// ********************************************************************************************** //
// Audio UDP Send Thread
// ********************************************************************************************** //
unsigned WINAPI AudioUDPSocketSendThread(void* pvParam)
{
	int AudioSN = 0;
	WORD wVersionRequested;

	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 1, 1 );
	if(WSAStartup( wVersionRequested, &wsaData )!=0)
	{
		cerr<<"Socket init error"<<endl;
#ifdef ERRCONTROL
		GlobalState=ERR_SEND_WSAStartup;     //����socket��������
#endif
		return 0;
	};

	SOCKET sockClient = socket( AF_INET , SOCK_DGRAM , 0 ) ;   //�����׽���

#ifdef ERRCONTROL
	if(sockClient == INVALID_SOCKE)
	{
		GlobalState=ERR_SEND_SOCKET_SETUP;     //����socket��������
		cerr<<"Socket client start error"<<endl;
		return 0;
	}
#endif

	SOCKADDR_IN addrSrv_local ;                       
	addrSrv_local.sin_addr.S_un.S_addr = INADDR_ANY;             
	addrSrv_local.sin_family = AF_INET ;                      
	addrSrv_local.sin_port = htons(LOCAL_SEND_PORT_Audio) ;                           //�������ڷ��͵Ķ˿�

	if(bind(sockClient,(struct sockaddr*)&addrSrv_local,sizeof(addrSrv_local)) == SOCKET_ERROR)  //�󶨱��ض˿�
	{
#ifdef ERRCONTROL
		GlobalState=ERR_SEND_BIND;     //����socket�󶨴���
#endif
		cerr<<"Bind error!"<<endl;
		return 0;
	}

	SOCKADDR_IN addrSrv ;                       
	addrSrv.sin_addr.S_un.S_addr = inet_addr(SGW_IP) ;     //SendIP��ַת��,sendIP�Ƿ���Ŀ��IP
	addrSrv.sin_family = AF_INET ;                             //ʹ��SendIP��ַ��
	addrSrv.sin_port = htons(SGW_PORT_Audio) ;                  //�˿�ת��		SENDPORT�Ƿ���Ŀ��PORT

	int len = sizeof(SOCKADDR);

	AMROutBuf		SocketAMRBuf;
	AudioSocketBuf	AudioSendBuf;

	Virtual_Audio_IP_Package mVirtual_Audio_IP_PackageS;

	memset( &SocketAMRBuf, 0, sizeof(SocketAMRBuf) );
	memset( &AudioSendBuf, 0, sizeof(AudioSendBuf) );

MSG msg;
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{if (msg.message==WM_TERM_THREAD) 
		{closesocket(sockClient) ;                        //�ر�
		WSACleanup();
		return 0;}
		}


		static BOOL pSocketSendQuequeEmpty;
		critical_section2.Lock();
		pSocketSendQuequeEmpty = pSocketSendQueque.empty();
		critical_section2.Unlock();

		if(!pSocketSendQuequeEmpty)
		{
			critical_section2.Lock();
			SocketAMRBuf = pSocketSendQueque.front();
			critical_section2.Unlock();
			AudioSN++;
			printf("oooooooooooooooooooo AudioSN = %d\n",AudioSN);

			// ********************************************************************************************** //


a_enc_package.RTP_Pack_Audio(SocketAMRBuf.AMR_analysis, &mVirtual_Audio_IP_PackageS  ,SocketAMRBuf.sequencenum );

			printf("len=%d\n",mVirtual_Audio_IP_PackageS.mDtpuhdr.wDtpUMsgLen);

			//  mVirtual_Audio_IP_PackageS ������� DTP��IP��UDP���
	a_enc_package.DtpIpUdp_Pack_Audio( &mVirtual_Audio_IP_PackageS );

			// memcpy(	AudioSendBuf.SocketData	,&mVirtual_Audio_IP_PackageS, mVirtual_Audio_IP_PackageS.mDtpuhdr.wDtpUMsgLen);
			// sizeof(mVirtual_Audio_IP_PackageS)  Ӧ�õ���  mVirtual_Audio_IP_PackageS.mDtpuhdr.wDtpUMsgLen

			memcpy(	AudioSendBuf.SocketData	,(unsigned char*)&mVirtual_Audio_IP_PackageS, sizeof(mVirtual_Audio_IP_PackageS));
			AudioSendBuf.datalen = sizeof(mVirtual_Audio_IP_PackageS);

			// ********************************************************************************************** //

			sendto( sockClient, (char*)AudioSendBuf.SocketData, AudioSendBuf.datalen, 0, (SOCKADDR*)&addrSrv, len );

			critical_section2.Lock();
			pSocketSendQueque.pop();
			critical_section2.Unlock();
		}
		Sleep(10);//��������Ĺ���CPU��Դ
	}

	closesocket(sockClient) ;                        //�ر�
	WSACleanup();

	printf("Thread OVER : AudioUDPSocketSendThread\n");
	return 0;
}


// ********************************************************************************************** //
// Video UDP Recevie Thread
// ********************************************************************************************** //
unsigned WINAPI VideoUDPSocketReceiveThread(void* pvParam)
{
	// ************************************************************************************************** //
	// ���л�����ʼ��
	// 
	int rcvnum = 0;
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 0 );
	WSAStartup( wVersionRequested, &wsaData );									//��winsocket��ʼ��

	SOCKET ReceiveSocket;														//�����׽���
	ReceiveSocket = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);					//���ݱ���UDP

	int iMode = 0;																//0��������1��������
	ioctlsocket(ReceiveSocket,FIONBIO,(u_long FAR*)&iMode);						//����Ϊ����ģʽ

	sockaddr_in   LocalAddr;
	sockaddr_in   RemoteAddr;
	int len = sizeof(SOCKADDR);

	LocalAddr.sin_family		= AF_INET;										// Internet
	LocalAddr.sin_port			= htons( LOCAL_RCV_PORT_Video);							// ����RCV_PORT == SGW_PORT   
	LocalAddr.sin_addr.s_addr	= INADDR_ANY;									// ����IP 
	bind( ReceiveSocket, (SOCKADDR*)&LocalAddr, len );							// �󶨱���IP�Ͷ˿�

//	unsigned char AMR_Rec[38];

	SocketBuf mSocket_In_Buf ;		//���ܵ���Buff 

	Video_H264		SocketH264Temp; // �������264����
	AMROutBuf		SocketAMRTemp ; // �������AMR����

	Virtual_Video_IP_Package mVirtual_Video_IP_PackageR;
	Virtual_Audio_IP_Package mVirtual_Audio_IP_PackageR;

	DTP_U_HDR	dtp_hdr;
	IPV4_HDR	ip_hdr;

	BOOL GetIpPackage = FALSE;
	BOOL RTPpackage = FALSE;

	BOOL PPS_frame = FALSE;
	BOOL SPS_frame = FALSE;
	BOOL IDR_frame = FALSE;

	Video_H264 SocketH264_SPS;
	Video_H264 SocketH264_PPS;
	Video_H264 SocketH264_IDR;
	memset(&SocketH264_SPS, 0, sizeof(SocketH264_SPS));
	memset(&SocketH264_PPS, 0, sizeof(SocketH264_PPS));
	memset(&SocketH264_IDR, 0, sizeof(SocketH264_IDR));

	memset( &mSocket_In_Buf, 0, sizeof(mSocket_In_Buf) );
	memset( &SocketH264Temp, 0, sizeof(SocketH264Temp) );
	memset( &SocketAMRTemp, 0, sizeof(SocketAMRTemp) );


	memset( &mVirtual_Video_IP_PackageR, 0, sizeof(mVirtual_Video_IP_PackageR) );
	memset( &mVirtual_Audio_IP_PackageR, 0, sizeof(mVirtual_Audio_IP_PackageR) );
	memset( &dtp_hdr, 0, sizeof(dtp_hdr) );
	memset( &ip_hdr, 0, sizeof(ip_hdr) );

	//
	// ��ʼ������
	// ************************************************************************************************** //
		 fd_set   fdsetRead; 
        timeval   tv; 
        tv.tv_sec=3;   //��ʱֵ3�� 
        tv.tv_usec=3000;//3000ms 
        int   nResult;    //��ʱ��־
		MSG msg;//**********����������ʱ
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{if (msg.message==WM_TERM_THREAD) 
		{closesocket(ReceiveSocket) ;                        //�ر�
		WSACleanup();
		return 0;}
		}
		// ************************************************************************************************** //
				FD_ZERO(&fdsetRead); //********���������ȴ�ʱ��
                FD_SET(ReceiveSocket,&fdsetRead); 
                nResult=select(0,&fdsetRead,NULL,NULL,&tv); 
                if(nResult==1) 
                { 
		mSocket_In_Buf.datalen = recvfrom( ReceiveSocket, (char*)mSocket_In_Buf.SocketData, MAX_VIDEO_UDP_PACKET, 0, (SOCKADDR*)&RemoteAddr, &len );
			 memcpy( &dtp_hdr, &mSocket_In_Buf.SocketData, sizeof(dtp_hdr) );
			if ( mSocket_In_Buf.datalen != SOCKET_ERROR )
			{
				rcvnum++;
				printf("UDPSocketReceiveThread num=%d\n",rcvnum);

				memcpy( &dtp_hdr, &mSocket_In_Buf.SocketData, sizeof(dtp_hdr) );
			

				memcpy( &ip_hdr, &mSocket_In_Buf.SocketData[8], 20 );
				// checksum IP
				ip_hdr.ip_offset = ntohs( ip_hdr.ip_offset );
				ip_hdr.ip_totallength = ntohs( ip_hdr.ip_totallength );

				unsigned short ip_RF_v_r		= ip_hdr.ip_offset >> 15;
				unsigned short ip_DF_v_r		= ( ip_hdr.ip_offset - (ip_RF_v_r << 15) ) >> 14;
				unsigned short ip_MF_v_r		= ( ip_hdr.ip_offset - (ip_RF_v_r << 15) - (ip_DF_v_r << 14) ) >> 13;
				unsigned short ip_OFFMASK_v_r	= ip_hdr.ip_offset - (ip_RF_v_r << 15) - (ip_DF_v_r << 14) - (ip_MF_v_r << 13);    // Ӧ����������λ������



				if ( ip_MF_v_r == 0 )	// ���һ����
				{
					printf("\n���һ����\n");

					if ( ip_OFFMASK_v_r == 0 )	// û�зְ�

					{printf("\n���� û�зְ�\n");

					memcpy( &mVirtual_Video_IP_PackageR, mSocket_In_Buf.SocketData, mSocket_In_Buf.datalen );}
					else
						memcpy( &mVirtual_Video_IP_PackageR.RTP_data[ ip_OFFMASK_v_r * 8 - 8], &mSocket_In_Buf.SocketData[8 + 20], ip_hdr.ip_totallength - 20 );

					// checksum UDP
					mVirtual_Video_IP_PackageR.mUdphdr.dst_portno = ntohs( mVirtual_Video_IP_PackageR.mUdphdr.dst_portno );


					GetIpPackage = TRUE;

				}
				else					// ���к�̰�
				{
					printf("\n���к�̰�\n");
					if ( ip_OFFMASK_v_r == 0 )	// ��һ����
						memcpy( &mVirtual_Video_IP_PackageR, mSocket_In_Buf.SocketData, mSocket_In_Buf.datalen );
					else
						memcpy( &mVirtual_Video_IP_PackageR.RTP_data[ ip_OFFMASK_v_r * 8 - 8], &mSocket_In_Buf.SocketData[8 + 20], ip_hdr.ip_totallength - 20 );
				}

				//	printf("\nmSocket_In_Buf.SocketDataΪ %d\n",mSocket_In_Buf.SocketData); 

				//------------------------------//---------------֮ǰ���-------------//



				if ( GetIpPackage       && dtp_hdr.dwDtpUIpId == LOCAL_IPID_Video) // mVirtual_Video_IP_PackageR.mUdphdr.dst_portno == LOCAL_RCV_PORT_Video
				{
					//		printf("\nGetIpPackage&dstPortno true\n"); 

					RTPpackage = TRUE;
				}
				else GetIpPackage = FALSE;


				//----------------???------

				if ( RTPpackage )
				{
					
					m_package.RTP_Depack_Video( &mVirtual_Video_IP_PackageR, &SocketH264Temp );



				//ID
					if ( IDR_frame )
					{
						critical_section5.Lock();
						pSocketRcvVideoDecQueue.push( SocketH264Temp );//�����յ���һ֡���ݵ��������
						critical_section5.Unlock();
					}
					else
					{
						int forbidden_bit		= SocketH264Temp.slicebuf[4] & 0x80; // 1 bit = 0
						int nal_reference_idc	= SocketH264Temp.slicebuf[4] & 0x60; // 2 bit = 11
						int nal_unit_type_s		= SocketH264Temp.slicebuf[4] & 0x1f; // 5 bit = SPS-7(00111);PPS-8(01000);IDR-5(00101);1(00001)

						//printf("NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  SocketH264Temp.slicebuf[4]);

						if ( SPS_frame && PPS_frame && nal_unit_type_s == 5 )
						{
							printf("NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  SocketH264Temp.slicebuf[4]);
							printf("SPS=%d .. PPS=%d .. IDR=%d\n",SPS_frame,PPS_frame,IDR_frame);
							critical_section5.Lock();
							pSocketRcvVideoDecQueue.push( SocketH264_SPS );//�����յ���һ֡���ݵ��������
							pSocketRcvVideoDecQueue.push( SocketH264_PPS );
							pSocketRcvVideoDecQueue.push( SocketH264Temp );
							critical_section5.Unlock();
							IDR_frame = TRUE;
						}
						else if ( !SPS_frame && nal_unit_type_s == 7 )
						{
							SPS_frame = TRUE;
							memcpy(&SocketH264_SPS, &SocketH264Temp, sizeof(SocketH264Temp));
							printf("NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  SocketH264Temp.slicebuf[4]);
							printf("SPS=%d .. PPS=%d .. IDR=%d\n",SPS_frame,PPS_frame,IDR_frame);
						}
						else if ( !PPS_frame && nal_unit_type_s == 8 )
						{
							PPS_frame = TRUE;
							memcpy(&SocketH264_PPS, &SocketH264Temp, sizeof(SocketH264Temp));
							printf("NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  SocketH264Temp.slicebuf[4]);
							printf("SPS=%d .. PPS=%d .. IDR=%d\n",SPS_frame,PPS_frame,IDR_frame);
						}

					}

					// *********************************************************************************************************************** //

					int forbidden_bit		= SocketH264Temp.slicebuf[4] & 0x80; // 1 bit = 0
					int nal_reference_idc	= SocketH264Temp.slicebuf[4] & 0x60; // 2 bit = 11
					int nal_unit_type_s		= SocketH264Temp.slicebuf[4] & 0x1f; // 5 bit = SPS-7(00111);PPS-8(01000);IDR-5(00101);1(00001)

					printf("NAL_TYPE is : %d .. %x\n",nal_unit_type_s,  SocketH264Temp.slicebuf[4]);

					if ( nal_unit_type_s == 7 )
					{
						critical_section5.Lock();
						pSocketRcvVideoDecQueue.push( SocketH264Temp );//�����յ���һ֡���ݵ��������
						critical_section5.Unlock();
						IDR_frame = TRUE;
					}
					//critical_section5.Lock();
					//pSocketRcvVideoDecQueue.push( SocketH264Temp );//�����յ���һ֡���ݵ��������
					//critical_section5.Unlock();

					// *********************************************************************************************************************** //
					RTPpackage = FALSE;
				}
				else printf("\n");

			}
			}
			//else printf("Socket Receive Error!!!\n");
		

		// ************************************************************************************************** //
		Sleep(1);
	}

	closesocket(ReceiveSocket);
	WSACleanup();

	printf("Thread OVER : UDPSocketReceiveThread\n");
	//EventVideoRcvEnd.SetEvent();
	return 0;
}


// ********************************************************************************************** //
// Video UDP Send Thread
// ********************************************************************************************** //

unsigned WINAPI VideoUDPSocketSendThread(void* pvParam)
{
// ************************************************************************************************** //
// ���л�����ʼ��
// 	

//	FILE *fp;
//	FILE *fpp;
	//FILE *fppp;


	int sendnum = 0;
	WORD wVersionRequested;
	
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 1, 1 );
	if(WSAStartup( wVersionRequested, &wsaData )!=0)
	{
		cerr<<"Video Socket init error"<<endl;
		return 1;
	};

	SOCKET sockClient = socket( AF_INET, SOCK_DGRAM, 0 );   // �����׽���

	SOCKADDR_IN addrSrv_local; 
	addrSrv_local.sin_addr.S_un.S_addr = INADDR_ANY;             
	addrSrv_local.sin_family = AF_INET;     

	while( 1 )
	{
		srand((unsigned long)time(0));
                
		addrSrv_local.sin_port = htons( rand()% 2000 + 10000 );                           // �������ڷ��͵Ķ˿�

		if (bind(sockClient,(struct sockaddr*)&addrSrv_local, sizeof(addrSrv_local)) != SOCKET_ERROR)
			break;
		else printf("\nVideo Bind error!\n\n");
	}

	//SOCKADDR_IN addrSrv_local;
	//addrSrv_local.sin_addr.S_un.S_addr = INADDR_ANY;             
	//addrSrv_local.sin_family = AF_INET;                      
	//addrSrv_local.sin_port = htons( LOCAL_SEND_PORT_Video );                           // �������ڷ��͵Ķ˿�

	//if( bind(sockClient,(struct sockaddr*)&addrSrv_local, sizeof(addrSrv_local)) == SOCKET_ERROR )  // �󶨱��ض˿�
	//{
	//	printf("\nVideo Bind error!\n\n");
	//	//cerr<<"Video Bind error!"<<endl;
	//	return -1;
	//}

	SOCKADDR_IN addrSrv;                       
	addrSrv.sin_addr.S_un.S_addr = inet_addr( SGW_IP );			// SendIP��ַת��,sendIP�Ƿ���Ŀ��IP
	addrSrv.sin_family = AF_INET;								// ʹ��SendIP��ַ��
	addrSrv.sin_port = htons(SGW_PORT_Video);						// �˿�ת��		SENDPORT�Ƿ���Ŀ��PORT

	int len = sizeof( SOCKADDR );

	Video_H264		SocketX264Temp;
	Video_H264 DepackH264file;
	VideoSocketBuf	VideoOutBuf;
	Virtual_Video_IP_Package mVirtual_Video_IP_PackageS;

	memset( &SocketX264Temp, 0, sizeof(SocketX264Temp) );
	memset( &VideoOutBuf, 0, sizeof(VideoOutBuf) );
	memset( &mVirtual_Video_IP_PackageS, 0, sizeof(mVirtual_Video_IP_PackageS) );
//
// ��ʼ������
// ************************************************************************************************** //

		MSG msg;
	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{if (msg.message==WM_TERM_THREAD) 
		{closesocket(sockClient) ;                        //�ر�
		WSACleanup();
		return 0;}
		}
		if ( VideoSendFlag )
		{
			//printf("lds1");
			static BOOL pVideoEncodeAndSendQueueEmpty;
			critical_section8.Lock();
			pVideoEncodeAndSendQueueEmpty = pVideoEncodeAndSendQueue.empty();
			critical_section8.Unlock();

			if(!pVideoEncodeAndSendQueueEmpty)
			{
				
				critical_section8.Lock();
				SocketX264Temp = pVideoEncodeAndSendQueue.front();
				critical_section8.Unlock();

				/*fp=fopen("C:\\X264B4skt.264","ab+");

				fwrite(SocketX264Temp.slicebuf,1,SocketX264Temp.slicelen,fp);
				fclose(fp);*/

				printf("Datalen== %d \n Data== %d\n ",SocketX264Temp.slicelen,SocketX264Temp.slicebuf);

				m_package.RTP_Pack_Video( &SocketX264Temp, &mVirtual_Video_IP_PackageS );
				printf("Rtp Datalen=== %d \n Data=== %d\n ",mVirtual_Video_IP_PackageS.data_len,mVirtual_Video_IP_PackageS.RTP_data);
				
				/*fp=fopen("C:\\X264B4sktRTP.264","rb+");
				fwrite(mVirtual_Video_IP_PackageS.RTP_data,1,SocketX264Temp.slicelen + 12,fp);
                fclose(fp);*/

				
				m_package.UDP_Pack_Video( &mVirtual_Video_IP_PackageS );
				
				

				//fppp=fopen("C:\\X264B4UDP.264","rb+");
				//fwrite(mVirtual_Video_IP_PackageS.RTP_data,1,SocketX264Temp.slicelen,fppp);
    //            fclose(fppp);
				
				
				while ( mVirtual_Video_IP_PackageS.data_len )
				{
					
					
					printf("\n data src len is : %d\n\n",mVirtual_Video_IP_PackageS.data_len);
					
					m_package.DtpIp_Pack_Video( &mVirtual_Video_IP_PackageS, &VideoOutBuf );

					printf("\nmVirtual_Video_IP_PackageS ��DTPͷ��IPͷ,UDPͷ�� ���ݳ���datalen, RTPdata��\n %X\n  %X\n  %X\n   %X\n  %X\n ", mVirtual_Video_IP_PackageS.mDtpuhdr,mVirtual_Video_IP_PackageS.mIphdr,mVirtual_Video_IP_PackageS.mUdphdr, mVirtual_Video_IP_PackageS.data_len,mVirtual_Video_IP_PackageS.RTP_data);

					//printf("\nafter DTPIP packed VideoOutbuf.socketData = %d\n \nVideoOutBuf.datalen==%d\n",VideoOutBuf.SocketData,VideoOutBuf.datalen );


					//printf("\nsend lost len is : %d\n\n",mVirtual_Video_IP_PackageS.data_len);

					//sendto( sockClient, (char *)&VideoOutBuf.SocketData[8], VideoOutBuf.datalen-8, 0, (SOCKADDR*)&addrSrv, len );

					//m_package.RTP_Depack_Video( &mVirtual_Video_IP_PackageS, &DepackH264file );


     //               FILE *fp;

					//fp=fopen("C:\\depack264!!!.264","ab+");

					//fwrite(DepackH264file.slicebuf,1,DepackH264file.slicelen,fp);

					//fclose(fp);



int si=	sendto( sockClient, (char *)VideoOutBuf.SocketData, VideoOutBuf.datalen, 0, (SOCKADDR*)&addrSrv, len );
					
					//printf("videoOutbuf.Socketdata===  %d \n   len === %d\n\n",mVirtual_Video_IP_PackageS.RTP_data, mVirtual_Video_IP_PackageS.data_len);

					

					sendnum++;
					
					printf("Video send Package num = %d .. package len = %d\n",sendnum,VideoOutBuf.datalen);
				
					//֮ǰȫ��OK...������...���ݴ�������
					
					memset( &VideoOutBuf, 0, sizeof(VideoOutBuf) );

                //mVirtual_Video_IP_PackageS ��mVirtual_Video_IP_PackageR  ò��һ��.
				}


				memset( &mVirtual_Video_IP_PackageS, 0, sizeof(mVirtual_Video_IP_PackageS) );

				critical_section8.Lock();
				pVideoEncodeAndSendQueue.pop();
				critical_section8.Unlock();

//////////////////////////////������///////////////////////////////////

	//VideoPlayFlag = TRUE;

	//VideoDecFlag = TRUE;

	//critical_section5.Lock();
	//pSocketRcvVideoDecQueue.push( SocketX264Temp );
	//critical_section5.Unlock();

////////////////////////////////////////////////////////////////////

			}
		}
		else
		{
			sendnum = 0;
			//printf("Socket Video Send Waiting\n");
		}

		Sleep(1);//��������Ĺ���CPU��Դ
	}

	closesocket(sockClient) ;                        //�ر�
	WSACleanup();

	printf("Thread OVER : VideoUDPSocketSendThread\n");
	//EventVideoSendEnd.SetEvent();
	return 0;
}