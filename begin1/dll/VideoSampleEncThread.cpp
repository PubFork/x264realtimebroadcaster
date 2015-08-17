#include "stdafx.h"
#include "VideoSampleEncThread.h"

//package v_enc_package;

//HWND m_hWnd;
//HWND m_hWndVideo;
//
//unsigned WINAPI CameraInit()
//{
//	hInst = hInstance;
//	int nShowCmd = 1;
//
//	WindowRegister( WndProc, (LPCWSTR)_T("Sample"), (HBRUSH)(COLOR_APPWORKSPACE + 1) );
//	m_hWnd = CreateWindow(_T("Sample"), _T("��׽��Ƶ"),
//		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
//		320, 240, 320, 275, NULL, NULL, hInstance, NULL);
//
//	if (!m_hWnd) return FALSE;
//
//	HWND hClient = GetWindow(m_hWnd, GW_CHILD);
//
//	m_hWndVideo = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE,1,1,320,240,m_hWnd,0);
//
//	//ShowWindow(m_hWnd, nShowCmd);
//
//	if ( CameraNewOpen )
//	{
//		CAPDRIVERCAPS gCapDriverCaps; //��Ƶ������������
//
//		//��ȡ��Ƶ�豸�ɼ�����    
//		capDriverGetCaps(m_hWndVideo,&gCapDriverCaps,sizeof(CAPDRIVERCAPS)); 
//
//		capDriverConnect(m_hWndVideo,0);
//		//���֧����ƵԴѡ���򵯳���ƵԴѡ��Ի���    
//		if(gCapDriverCaps.fHasDlgVideoSource) 
//		{
//			capDlgVideoSource(m_hWndVideo);  
//		}
//		//���֧����Ƶ��ʽѡ���򵯳���Ƶ��ʽѡ��Ի���   
//		if(gCapDriverCaps.fHasDlgVideoFormat)
//		{
//			capDlgVideoFormat(m_hWndVideo);  
//		}
//		//���֧����Ƶ��ʾ��ʽѡ���򵯳���Ƶ��ʾ��ʽѡ��Ի���    
//		if(gCapDriverCaps.fHasDlgVideoDisplay)   
//		{
//			capDlgVideoDisplay(m_hWndVideo);
//		}
//		CameraNewOpen = FALSE;
//	}
//
//printf("CameraInit ok \n");
//
//	MSG msg;
//
//	while (GetMessage(&msg, NULL, 0, 0))
//	{
//		if (!TranslateMDISysAccel(hClient, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//	return msg.wParam;
//}
//
//
//unsigned WINAPI CameraSmapleOpen()
//{
//	printf("Camera Sample is Ready\n");
//	//��ò��������������ܲ���
//	CAPDRIVERCAPS CapDrvCaps;
//	capDriverGetCaps(m_hWndVideo, &CapDrvCaps, sizeof(CAPDRIVERCAPS));
//	//������Ƶ��׽����
//	CAPTUREPARMS CapParms;
//	capCaptureGetSetup(m_hWndVideo, &CapParms, sizeof(CAPTUREPARMS));
//	// ��������
//	CapParms.dwRequestMicroSecPerFrame = (DWORD)(1.0e6/25);
//	// ����ʱ������ 
//	CapParms.fLimitEnabled = FALSE; 
//	// �Ƿ�׽��Ƶ
//	// CapParms.fCaptureAudio = FALSE; 
//	// MCI Device֧��
//	CapParms.fMCIControl = FALSE;  
//	// ���ô���,���Ϊfalse,��׽������������              
//	CapParms.fYield = TRUE;
//	// ֹͣ��׽������
//	CapParms.vKeyAbort			= VK_ESCAPE;
//	CapParms.fAbortLeftMouse	= FALSE;
//	CapParms.fAbortRightMouse	= FALSE;
//	capCaptureSetSetup(m_hWndVideo, & CapParms, sizeof(CAPTUREPARMS));
//
//	//������������
//	if( capDriverConnect(m_hWndVideo,0) )
//	{
//		::SetParent(m_hWndVideo,m_hWnd);
//		::SetWindowLong(m_hWndVideo,GWL_STYLE,WS_CHILD);
//
//		//��ò��񴰿�״̬(Status)
//		CAPSTATUS CapStatus;
//		capGetStatus(m_hWndVideo, &CapStatus, sizeof (CAPSTATUS));
//		SetWindowPos(m_hWndVideo, NULL, 0, 0, CapStatus.uiImageWidth, CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);
//
//		//���ûص�����capVideoStreamCallback��ÿ�ɼ�һ֡���ûص�����һ��
//		capSetCallbackOnVideoStream(m_hWndVideo, &capVideoStreamCallback);
//
//		//���в�׽�����洢�ļ�
//		capCaptureSequenceNoFile(m_hWndVideo);
//	}
//printf("Camera Sample is Done\n");
//	return 0;
//}
//
//
//unsigned WINAPI CameraSampleClose()
//{
//printf("Camera Sample Will Close\n");
//	capCaptureStop(m_hWndVideo);
//	capCaptureAbort(m_hWndVideo);
//	capSetCallbackOnVideoStream(m_hWndVideo,NULL);
//	Sleep (300);
//	capDriverDisconnect(m_hWndVideo);
//	//EventVideoSampleEnd.SetEvent();
//printf("Camera Sample is Closed\n");
//	return 0;
//}


unsigned WINAPI VideoSampleThread(void* pvParam)
{
	printf("Running:      VideoSampleThread\n");
	//CameraInit();
	CameraSample();
	printf("Thread OVER : VideoSampleThread\n");
	return 0;
}


unsigned WINAPI H264EncodeAndPackageThread(void* pvParam)
{
// ******************************************************************** //
// �������弰��ʼ��
//
//FILE *fp;

    x264_param_t param;
    x264_t *h;
    x264_picture_t pic;

    x264_picture_t pic_out;
    x264_nal_t *nal;
    int i_nal, i;
    int i_file = 0;

    int	i_frame = 0;
	short videonum = 0;

	Video_YUV420	YUVFrameTemp;
	Video_H264		X264Temp;

	memset( &YUVFrameTemp, 0, sizeof(YUVFrameTemp) );
	memset( &X264Temp, 0, sizeof(X264Temp) );

    x264_param_default( &param );	//�Ա��������в����趨����ʼ��������

    if( ( h = x264_encoder_open( &param ) ) == NULL )		//��������ǶԲ���ȷ�Ĳ��������޸ģ����Ը��ṹ�������CABAC���롢Ԥ�����Ҫ�Ĳ������г�ʼ������ʼ������������
    {
        fprintf( stderr, "x264 [error]: x264_encoder_open failed\n" );
        return -1;
    }

    x264_picture_alloc( &pic, X264_CSP_I420, param.i_width, param.i_height );		//Ϊ��ͼ����ռ�

// 
// �������弰��ʼ��
// ******************************************************************** //

/// Encode frames ///
	while( 1 ) //VideoEncodFlag
	{
		static BOOL pVideoSampleAndEncodeQueueEmpty;
		critical_section7.Lock();
		pVideoSampleAndEncodeQueueEmpty = pVideoSampleEncQueue.empty();
	//	printf("          pVideoSampleEncQueue.size()=%d\n",pVideoSampleEncQueue.size());
		critical_section7.Unlock();
		if( !pVideoSampleAndEncodeQueueEmpty && VideoEncodFlag )
		{

///////////////////////////////////////////////////////////////////////////////////////////////////////
	//static int pVideoSampleAndEncodeQueuesize;
	//critical_section7.Lock();
	//pVideoSampleAndEncodeQueuesize = pVideoSampleEncQueue.size();
	//critical_section7.Unlock();
	//printf("pVideoSampleAndEncodeQueuesize = %d\n",pVideoSampleAndEncodeQueuesize);
///////////////////////////////////////////////////////////////////////////////////////////////////////	
			critical_section7.Lock();
			YUVFrameTemp = pVideoSampleEncQueue.front();
			critical_section7.Unlock();

			memcpy( pic.img.plane[0], YUVFrameTemp.YUVbuffer, YUVFrameTemp.width * YUVFrameTemp.height );
			memcpy( pic.img.plane[1], YUVFrameTemp.YUVbuffer + YUVFrameTemp.width * YUVFrameTemp.height, YUVFrameTemp.width * YUVFrameTemp.height / 4 );
			memcpy( pic.img.plane[2], YUVFrameTemp.YUVbuffer + YUVFrameTemp.width * YUVFrameTemp.height * 5 / 4, YUVFrameTemp.width * YUVFrameTemp.height / 4 );

			pic.i_pts = i_frame * param.i_fps_den;
			pic.i_type = X264_TYPE_AUTO;
			pic.i_qpplus1 = 0;

// ********************************************************************************************** //

			memset( &X264Temp, 0, sizeof(X264Temp) );

			x264_encoder_encode( h, &nal, &i_nal, &pic, &pic_out );

			for( i = 0; i < i_nal; i++ )
			{
				//printf("program test - nal.i_type %d\n",nal[i].i_type);
				x264_nal_encode( X264Temp.slicebuf, &X264Temp.slicelen, 1, &nal[i] ); //����NAL�㣬����������
			
				
				printf("264encode Datam: %X ",&X264Temp.slicebuf);
				printf("264encode Datam: %s ",&X264Temp.slicebuf);
                printf("264encode Datam: %X ",&X264Temp.slicebuf);
				
					
					if( X264Temp.slicelen > 0 )
				{
// ************************************************************************* //
					//int n;
					//for( n = 0; n < 8; n++)
					//{
					//	printf("ENC :%02X ",X264Temp.slicebuf[n]);
					//}
					//printf("\n\n");
		
					
					/*fp=fopen("C:\\pcms.wav", "ab+");
                    fwrite(pBufferIn[BufferInNo], 1, 320, fp);
                    fclose(fp);*/
					
					
					
     //               FILE *fp;

					//fp=fopen("C:\\264encode.264","ab+");

					//fwrite(X264Temp.slicebuf,1,X264Temp.slicelen,fp);

					//fclose(fp);
//
				//	printf("\nX264 Encode slicebuf �Լ� SQ TS�� slice len= %X\n�� %X\n �� %X\n�� %X\n ",X264Temp.slicebuf, X264Temp.sq, X264Temp.ts, X264Temp.slicelen);
					
					// ************************************************************************* //
				//	printf("X264 Encode slice len = %d\n",X264Temp.slicelen);

					X264Temp.sq = videonum;
					videonum ++;
					critical_section8.Lock();
					pVideoEncodeAndSendQueue.push( X264Temp );
					critical_section8.Unlock();
				}
			}

// ********************************************************************************************** //

			i_frame ++;
			printf("X264 Encoding - %d\n",i_frame);

			critical_section7.Lock();
			pVideoSampleEncQueue.pop();
			critical_section7.Unlock();
		}

		Sleep (1);
	}

    x264_picture_clean( &pic );		//����ͼƬ��  error unknown
    x264_encoder_close( h );		//�رձ�����
	printf("Thread OVER : H264EncodeAndPackageThread\n");
	//EventVideoEncodEnd.SetEvent();
	
	return 0;
}

