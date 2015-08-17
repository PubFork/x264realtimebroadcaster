#include "stdafx.h"
#include "VideoSample.h"


/****************************************��Ƶ��׽ÿһ֡��ͬʱ�����ô˺���*******************************************************/
LRESULT CALLBACK capVideoStreamCallback(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	Video_YUV420 yuv_buf;
	memset(&yuv_buf, 0, sizeof(yuv_buf));

	yuv_buf.width = CWIDTH;
	yuv_buf.height = CHEIGHT;

	if ( lpVHdr->dwFlags & VHDR_DONE && VideoSampleFlag ) 
	{
		unsigned char * pSrc	= (unsigned char*)calloc(2 * WIDTH * HEIGHT,sizeof(char));
		unsigned char * pDest	= (unsigned char*)calloc(3 * WIDTH * HEIGHT,sizeof(char));
		unsigned char * rgb_buf = (unsigned char*)calloc(3 * yuv_buf.width * yuv_buf.height,sizeof(char));
		//lpVHdr->lpDataΪ��׽��YUY2ͼ������
		memcpy(pSrc, lpVHdr->lpData, 2*WIDTH*HEIGHT);
		//lpVHdr->lpDataָ�򲶻�RGB�Ŀռ��ַ

		YUY2_RGB( pDest, pSrc, WIDTH, HEIGHT );

		if( CWIDTH == WIDTH && CHEIGHT == HEIGHT )
			memcpy( rgb_buf, pDest, 3 * WIDTH * HEIGHT );
		else
			RGBCUT( rgb_buf, pDest, yuv_buf.width, yuv_buf.height, WIDTH,HEIGHT );

		ConvertRGBtoYUV420P(rgb_buf, yuv_buf.YUVbuffer, yuv_buf.width, yuv_buf.height);

 // *********************************************************************************************** //
		static int pVideoSampleEncQueueSize;
		critical_section7.Lock();
		pVideoSampleEncQueueSize = pVideoSampleEncQueue.size();
		critical_section7.Unlock();

		if( pVideoSampleEncQueueSize < 5 )
		{
			if ( !PLAYRCVONLY )
			{
				YUVdisplayerM( yuv_buf.width, yuv_buf.height, yuv_buf.YUVbuffer );

				if( VideoRecordFlag )
				{	
					printf("Video Record Thread Start!\n");
					YUVSmpRecordFlag = TRUE;
					TempLenYUVS = 16 + yuv_buf.width * yuv_buf.height * 3/2;
					memcpy(pTempYUVS, &yuv_buf, TempLenYUVS);
					EventRecodYUVS.SetEvent();
				}
			}
			critical_section7.Lock();
			pVideoSampleEncQueue.push( yuv_buf );
			critical_section7.Unlock();
		}
		else
		{
			critical_section7.Lock();
			for(int queueCount = 0; queueCount<2; queueCount++)
			{
				pVideoSampleEncQueue.pop();
			}
			critical_section7.Unlock();
		}
 // *********************************************************************************************** //

		//critical_section7.Lock();
		//pVideoSampleEncQueue.push(yuv_buf);
		//critical_section7.Unlock();

		free(pSrc);
		free(pDest);
		free(rgb_buf);
	}
	//else
	//{
	//	num = 0;
	//	//printf("Video Sample Waiting\n");
	//}

	return (LRESULT) TRUE;
}

HINSTANCE hInstance;
HINSTANCE hInst;

ATOM WindowRegister(WNDPROC lpfnWndProc, LPCWSTR lpClname, HBRUSH hbrBack)
{
	WNDCLASSEX	wnd;

	wnd.cbClsExtra		= 0;
	wnd.cbSize			= sizeof(WNDCLASSEX);
	wnd.cbWndExtra		= 0;
	wnd.hbrBackground	= hbrBack;
	wnd.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wnd.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance		= hInst;
	wnd.lpfnWndProc		= lpfnWndProc;
	wnd.lpszClassName	= _T("Sample");
	wnd.lpszMenuName	= NULL;
	wnd.style			= CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hClient;

	switch (uMsg)
	{
		case WM_CREATE:
			return 0;

		case WM_DESTROY: 
			PostQuitMessage(0); 
			return 0;

		case WM_PAINT:
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case 1000:
					break;

				default:
					break;
			}
			break;
	}

	return DefFrameProc(hWnd, hClient, uMsg, wParam, lParam);
}


INT WINAPI CameraSample()
{
	YUVSmpRecordFlag = FALSE;

	hInst = hInstance;

	WindowRegister( WndProc, (LPCWSTR)_T("Sample"), (HBRUSH)(COLOR_APPWORKSPACE + 1) );
	HWND m_hWnd = CreateWindow(_T("Sample"), _T("��׽��Ƶ"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
		300, 200, 352, 288, NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	HWND hClient = GetWindow(m_hWnd, GW_CHILD);

// **********************************************��Ƶ��׽����*************************************************** //
	//����Ԥ�����ڣ���Ҫ�����ڵľ��m_hWnd��NULL
	HWND m_hWndVideo;
	m_hWndVideo = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE,1,1,CWIDTH,CHEIGHT,m_hWnd,0);

///////////////////////////���ο���ͨ�������Ի���ѡ��ɼ�Դ���ɼ���ʽ���ֱ��ʵ�//////////////////////////////////////
	
		CAPDRIVERCAPS gCapDriverCaps; //��Ƶ������������  
		CAPSTATUS gCapStatus; //���񴰵�״̬   
		////��ȡ��Ƶ�豸�ɼ�����    
		capDriverGetCaps( m_hWndVideo, &gCapDriverCaps, sizeof(CAPDRIVERCAPS) ); 

		capDriverConnect(m_hWndVideo,0);
		////���֧����ƵԴѡ���򵯳���ƵԴѡ��Ի���    
		//if(gCapDriverCaps.fHasDlgVideoSource) 
		//{
		capDlgVideoSource(m_hWndVideo);  
		//}
		////���֧����Ƶ��ʽѡ���򵯳���Ƶ��ʽѡ��Ի���   
		//if(gCapDriverCaps.fHasDlgVideoFormat)
		//{
		capDlgVideoFormat( m_hWndVideo );
		//capDlgVideoFormat( hClient );
		//capDlgVideoFormat( m_hWnd );
		//	//capGetVideoFormatSize( m_hWndVideo ); 
		//}
		////���֧����Ƶ��ʾ��ʽѡ���򵯳���Ƶ��ʾ��ʽѡ��Ի���    
		//if(gCapDriverCaps.fHasDlgVideoDisplay)   
		//{
			capDlgVideoDisplay(m_hWndVideo);
		//}

// ************************************************************************************************************************* //

	//��ò��������������ܲ���
	CAPDRIVERCAPS CapDrvCaps;
	capDriverGetCaps(m_hWndVideo, &CapDrvCaps, sizeof(CAPDRIVERCAPS));
	//������Ƶ��׽����
	CAPTUREPARMS CapParms;
	capCaptureGetSetup(m_hWndVideo, &CapParms, sizeof(CAPTUREPARMS));
	// ��������
	//CapParms.dwRequestMicroSecPerFrame = 400000;
	CapParms.dwRequestMicroSecPerFrame = (DWORD)(1.0e6/25);
	// ����ʱ������ 
	CapParms.fLimitEnabled = FALSE; 
	// �Ƿ�׽��Ƶ
	// CapParms.fCaptureAudio = FALSE; 
	// MCI Device֧��
	CapParms.fMCIControl = FALSE;  
	// ���ô���,���Ϊfalse,��׽������������              
	CapParms.fYield = TRUE;
	// ֹͣ��׽������
	CapParms.vKeyAbort			= VK_ESCAPE;
	CapParms.fAbortLeftMouse	= FALSE;
	CapParms.fAbortRightMouse	= FALSE;
	capCaptureSetSetup(m_hWndVideo, & CapParms, sizeof(CAPTUREPARMS));

	//������������
	if( capDriverConnect(m_hWndVideo,0) )
	{
		::SetParent(m_hWndVideo,m_hWnd);
		::SetWindowLong(m_hWndVideo,GWL_STYLE,WS_CHILD);
		//��ò��񴰿�״̬(Status)
		CAPSTATUS CapStatus;
		capGetStatus(m_hWndVideo, &CapStatus, sizeof (CAPSTATUS));
		WIDTH = CapStatus.uiImageWidth;
		HEIGHT = CapStatus.uiImageHeight;
		printf("CapStatus.uiImageWidth = %d .. CapStatus.uiImageHeight = %d\n",CapStatus.uiImageWidth,CapStatus.uiImageHeight);

		SetWindowPos(m_hWndVideo, NULL, 0, 0, CapStatus.uiImageWidth, CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

		//���ûص�����capVideoStreamCallback��ÿ�ɼ�һ֡���ûص�����һ��
		capSetCallbackOnVideoStream(m_hWndVideo, &capVideoStreamCallback);
		//���в�׽�����洢�ļ�
		capCaptureSequenceNoFile(m_hWndVideo);
	}
// **********************************************��Ƶ��׽���ֽ���*************************************************** //
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateMDISysAccel(hClient, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



void YUY2_RGB(BYTE *pDstData, BYTE *pSrcData, int nWidth, int nHeight)
{
	///@note RGB24��ʽ����������
	int R,G,B;                                                           
	int x(0), y(0);
	int i=0;
	///@note YUV2��ʽ���ĸ�����
	long Y0(0), U(0), Y1(0), V(0);                                          
	for ( y = 0; y <nHeight; y++)
	{
		for ( x = 0; x < nWidth; x += 2)
		{
			///@note YUY2��ʽ�ķ�����ֵ
			Y0 = *pSrcData++;
			U  = *pSrcData++;
			Y1 = *pSrcData++;
			V  = *pSrcData++;
   
   ////////////////////////��ʽ������/////////////////////////////////////////
			R  = (int)(1.164383 * (Y0 - 16) + 1.596027 * (V - 128));
			G  = (int)(1.164383 * (Y0 - 16) - 0.812968 * (V - 128) - 0.391762 * (U - 128));
			B  = (int)(1.164383 * (Y0 - 16) + 2.017232 * (U - 128));
 
			if ( R < 0 )
			{
				R = 0;
			}
			if ( R > 255)
			{
				R = 255;
			}
			if ( G < 0 )
			{
				G = 0;
			}
			if ( G > 255)
			{
				G = 255;
			}
			if ( B < 0 )
			{
				B = 0;
			}
			if ( B > 255 )
			{
				B = 255;
			}
			*pDstData++ = ( BYTE )B;
			*pDstData++ = ( BYTE )G;
			*pDstData++ = ( BYTE )R;
   
			R  = (int)(1.164383 * (Y1 - 16) + 1.596027 * (V - 128));
			G  = (int)(1.164383 * (Y1 - 16) - 0.812968 * (V - 128) - 0.391762 * (U - 128));
			B  = (int)(1.164383 * (Y1 - 16) + 2.017232 * (U - 128));

			if ( R < 0 )
			{
				R = 0;
			}
			if ( R > 255)
			{
				R = 255;
			}
			if ( G < 0 )
			{
				G = 0;
			}
			if ( G > 255)
			{
				G = 255;
			}
			if ( B < 0 )
			{
				B = 0;
			}
			if ( B > 255 )
			{
				B = 255;
			}
			*pDstData++ = ( BYTE )B;
			*pDstData++ = ( BYTE )G;
			*pDstData++ = ( BYTE )R;
		}
	}
}


//����ȡ��Ƶͼ�����ϽǼ�����������Ϊ�ֱ��ʸ�С����Ƶ
void RGBCUT(unsigned char *rgb_buf,unsigned char *rgb_buf0,int cWidth,int cHeight,int Width,int Height )
{
	int i=0;int j=0;int i1=0;int j1=0;

	//for( i=0; i<cHeight; i++ )
	//{
	//	for(j=0;j<cWidth;j++)
	//	{
	//		*(rgb_buf+3*i*cWidth+3*j)=*(rgb_buf0+3*(i+(Height-cHeight)/2)*Width+3*(j+(Width-cWidth)/2));
	//		*(rgb_buf+3*i*cWidth+3*j+1)=*(rgb_buf0+3*(i+(Height-cHeight)/2)*Width+3*(j+(Width-cWidth)/2)+1);
	//		*(rgb_buf+3*i*cWidth+3*j+2)=*(rgb_buf0+3*(i+(Height-cHeight)/2)*Width+3*(j+(Width-cWidth)/2)+2);
	//	}	
	//}

	for(i1=0;i1<cHeight;i1++)
	{
		j=0;
		if(i1==0)
			i=0;
		else
			i=(i1+1)*Height/cHeight-1;
		for(j1=0;j1<cWidth;j1++)
		{
			if(j1==0)
				j=0;
			else
				j=(j1+1)*Width/cWidth-1;

			*(rgb_buf + 3*i1*cWidth + 3*j1)		=*(rgb_buf0 + 3*i*Width + 3*j);
			*(rgb_buf + 3*i1*cWidth + 3*j1 + 1)	=*(rgb_buf0 + 3*i*Width + 3*j + 1);
			*(rgb_buf + 3*i1*cWidth + 3*j1 + 2)	=*(rgb_buf0 + 3*i*Width + 3*j + 2);
		}

	}
	
} 


//////RGBתΪYUV����
void ConvertRGBtoYUV420P(const unsigned char *rgb, unsigned char *yuv, int width, int height)
{
	// change from yuv to rgb, using averaging method.....
	// to get better edge
	unsigned char *y, *u, *v;
	const unsigned char *p, *q;
	int i, j;
	int r, g, b;

	const int k = 3 * width + 3; /* the left, top pixel */

	y = yuv;
	u = y + width * height;
	v = u + ( (width * height)>>2 );
	p = rgb;
	
	for(j=0; j<height; j++)
	{
		for(i=0; i<width; i++)
		{
			//rgbtoyuv_y( *(p+2), *(p+1), *(p), *y++ );
			rgbtoyuv_y( *(p), *(p+1), *(p+2), *y++ );

			if ((i&1) && (j&1))
			{
				q = p-k;
				r = (((int)*(q+0))+*(q+3)+*(p-3)+*(p+0))>>2;
				g = (((int)*(q+1))+*(q+4)+*(p-2)+*(p+1))>>2;
				b = (((int)*(q+2))+*(q+5)+*(p-1)+*(p+2))>>2;
				rgbtoyuv_u(r, g, b, *u++);
				rgbtoyuv_v(r, g, b, *v++);
			}
			p+=3;
		}
	}
}
