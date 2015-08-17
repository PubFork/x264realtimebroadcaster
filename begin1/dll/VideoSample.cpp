#include "stdafx.h"
#include "VideoSample.h"


/****************************************视频捕捉每一帧的同时，调用此函数*******************************************************/
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
		//lpVHdr->lpData为捕捉到YUY2图像数据
		memcpy(pSrc, lpVHdr->lpData, 2*WIDTH*HEIGHT);
		//lpVHdr->lpData指向捕获RGB的空间地址

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
	HWND m_hWnd = CreateWindow(_T("Sample"), _T("捕捉视频"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
		300, 200, 352, 288, NULL, NULL, hInstance, NULL);

	if (!m_hWnd) return FALSE;

	HWND hClient = GetWindow(m_hWnd, GW_CHILD);

// **********************************************视频捕捉部分*************************************************** //
	//创建预览窗口，需要父窗口的句柄m_hWnd或NULL
	HWND m_hWndVideo;
	m_hWndVideo = capCreateCaptureWindow(NULL,WS_CHILD|WS_VISIBLE,1,1,CWIDTH,CHEIGHT,m_hWnd,0);

///////////////////////////本段可以通过弹出对话框选择采集源、采集格式、分辨率等//////////////////////////////////////
	
		CAPDRIVERCAPS gCapDriverCaps; //视频驱动器的能力  
		CAPSTATUS gCapStatus; //捕获窗的状态   
		////获取视频设备采集能力    
		capDriverGetCaps( m_hWndVideo, &gCapDriverCaps, sizeof(CAPDRIVERCAPS) ); 

		capDriverConnect(m_hWndVideo,0);
		////如果支持视频源选择，则弹出视频源选择对话框    
		//if(gCapDriverCaps.fHasDlgVideoSource) 
		//{
		capDlgVideoSource(m_hWndVideo);  
		//}
		////如果支持视频格式选择，则弹出视频格式选择对话框   
		//if(gCapDriverCaps.fHasDlgVideoFormat)
		//{
		capDlgVideoFormat( m_hWndVideo );
		//capDlgVideoFormat( hClient );
		//capDlgVideoFormat( m_hWnd );
		//	//capGetVideoFormatSize( m_hWndVideo ); 
		//}
		////如果支持视频显示格式选择，则弹出视频显示格式选择对话框    
		//if(gCapDriverCaps.fHasDlgVideoDisplay)   
		//{
			capDlgVideoDisplay(m_hWndVideo);
		//}

// ************************************************************************************************************************* //

	//获得捕获驱动器的性能参数
	CAPDRIVERCAPS CapDrvCaps;
	capDriverGetCaps(m_hWndVideo, &CapDrvCaps, sizeof(CAPDRIVERCAPS));
	//设置视频捕捉参数
	CAPTUREPARMS CapParms;
	capCaptureGetSetup(m_hWndVideo, &CapParms, sizeof(CAPTUREPARMS));
	// 设置桢速
	//CapParms.dwRequestMicroSecPerFrame = 400000;
	CapParms.dwRequestMicroSecPerFrame = (DWORD)(1.0e6/25);
	// 有无时间限制 
	CapParms.fLimitEnabled = FALSE; 
	// 是否捕捉音频
	// CapParms.fCaptureAudio = FALSE; 
	// MCI Device支持
	CapParms.fMCIControl = FALSE;  
	// 设置窗口,如果为false,捕捉画面在桌面上              
	CapParms.fYield = TRUE;
	// 停止捕捉键设置
	CapParms.vKeyAbort			= VK_ESCAPE;
	CapParms.fAbortLeftMouse	= FALSE;
	CapParms.fAbortRightMouse	= FALSE;
	capCaptureSetSetup(m_hWndVideo, & CapParms, sizeof(CAPTUREPARMS));

	//连接驱动程序
	if( capDriverConnect(m_hWndVideo,0) )
	{
		::SetParent(m_hWndVideo,m_hWnd);
		::SetWindowLong(m_hWndVideo,GWL_STYLE,WS_CHILD);
		//获得捕获窗口状态(Status)
		CAPSTATUS CapStatus;
		capGetStatus(m_hWndVideo, &CapStatus, sizeof (CAPSTATUS));
		WIDTH = CapStatus.uiImageWidth;
		HEIGHT = CapStatus.uiImageHeight;
		printf("CapStatus.uiImageWidth = %d .. CapStatus.uiImageHeight = %d\n",CapStatus.uiImageWidth,CapStatus.uiImageHeight);

		SetWindowPos(m_hWndVideo, NULL, 0, 0, CapStatus.uiImageWidth, CapStatus.uiImageHeight, SWP_NOZORDER | SWP_NOMOVE);

		//设置回调函数capVideoStreamCallback，每采集一帧调用回调函数一次
		capSetCallbackOnVideoStream(m_hWndVideo, &capVideoStreamCallback);
		//进行捕捉，不存储文件
		capCaptureSequenceNoFile(m_hWndVideo);
	}
// **********************************************视频捕捉部分结束*************************************************** //
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
	///@note RGB24格式的三个分量
	int R,G,B;                                                           
	int x(0), y(0);
	int i=0;
	///@note YUV2格式的四个分量
	long Y0(0), U(0), Y1(0), V(0);                                          
	for ( y = 0; y <nHeight; y++)
	{
		for ( x = 0; x < nWidth; x += 2)
		{
			///@note YUY2格式四分量赋值
			Y0 = *pSrcData++;
			U  = *pSrcData++;
			Y1 = *pSrcData++;
			V  = *pSrcData++;
   
   ////////////////////////公式有问题/////////////////////////////////////////
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


//将获取视频图像左上角剪切下来，成为分辨率更小的视频
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


//////RGB转为YUV数据
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
