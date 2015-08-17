#include "stdafx.h"
#include "yuvdisplay.h"

//#include "GlobalVariable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//YUV转为RGB数组
void ConvertYUV420PToRGB888(const unsigned char *yuv, unsigned char *rgb, int width, int height)
{
	// assume rgb at least width*height*3
	// assuem yuv at least width*height+ width*height/4 + width*height/4\
	// yuv and rgb can be the same buffer!
	int j, i, k;
	const unsigned planeSize = width*height;
	const unsigned char *y, *u, *v;	
	unsigned char *p;
	
	y = yuv;
	p = rgb;
	for(j=0; j<height; j++)
	{
		u = (yuv+planeSize)+(j>>1)*(width>>1);
		v = (yuv+planeSize+(planeSize>>2))+(j>>1)*(width>>1);
		for(i=0; i<width; i++)
		{
			//yuvtorgb(*y, *u, *v, *(p+2), *(p+1), *(p), k);
			yuvtorgb(*y, *u, *v, *(p), *(p+1), *(p+2), k);
			p += 3;
			y ++;
			u += (i&1);
			v += (i&1);
		}
	}
}

//RGB转HBITMAP函数
HBITMAP RGB2HBITMAP(int nWidth, int nHeight, unsigned char* rgb_buf)
{
	int size;
	unsigned char* pRGB;
	pRGB = rgb_buf;
	size = nWidth*nHeight*3;
	//Sleep(40);
	HDC hDC = ::GetDC(0);

	//写位图信息头
	BITMAPINFOHEADER bmih;
	memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = nWidth;
	bmih.biHeight = -nHeight;
	bmih.biSizeImage = nWidth*nHeight;
	bmih.biCompression = BI_RGB;
	bmih.biBitCount = 24;
	bmih.biPlanes = 1;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	void* pBits;
	BITMAPINFO bmi;
	bmi.bmiHeader = bmih;
	HBITMAP hBmp ;
	//建立位图
	hBmp = CreateDIBSection(hDC ,&bmi , DIB_RGB_COLORS, &pBits, NULL, 0);
	int ret = SetDIBits(hDC, hBmp, 0, nHeight, pRGB, &bmi, DIB_RGB_COLORS);

	ReleaseDC(0,hDC);
	return hBmp;
}


//YUV显示主函数
int YUVdisplayer( int nWidth, int nHeight, unsigned char* yuvbuf)
{
if(tab_Mode==0)
{if(H_local==0)
{	HDC hdc =::GetDC((HWND)HANDLE_DISPLAY);
	HDC hdcMem = CreateCompatibleDC(hdc);
	char *rgbbuf = (char *)malloc(((nWidth*3+3)&~3)*nHeight+nWidth*nHeight);
	unsigned char *rgb_buf = (unsigned char*)(rgbbuf); 
	unsigned char *yuv_buf = (unsigned char*)(yuvbuf);
	
	if (rgb_buf)
	{
		ConvertYUV420PToRGB888(yuv_buf, rgb_buf, nWidth, nHeight);
	}
	HBITMAP hBmp = RGB2HBITMAP(nWidth, nHeight,rgb_buf);	
	//Select the bitmap into to the compatible device context
    HGDIOBJ hOldSel = SelectObject(hdcMem,hBmp);
    //Get the bitmap dimensions from the bitmap
    BITMAP bmp;
    GetObject(hBmp,sizeof(BITMAP),&bmp);
 
    //Copy the bitmap image from the memory DC to the screen DC

	StretchBlt(hdc, coordinate_x, coordinate_y, wsize, hsize, 
		hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);//StretchBlt可将图像复制到显示区域，并按所设参数（352,288）进行缩放。

	//Restore original bitmap selection and destroy the memory DC
    SelectObject(hdcMem,hOldSel);
	DeleteDC(hdcMem);
	::DeleteObject(hBmp);
	DeleteDC(hdc);
	free(rgbbuf);
	return 0;
}
}
else
return 0;
}

//YUV显示主函数
int YUVdisplayerM( int nWidth, int nHeight, unsigned char* yuvbuf)
{if(tab_Mode==0)
{if(H_local==1)
{HDC hdc =::GetDC((HWND)AHANDLE_DISPLAY);
	HDC hdcMem = CreateCompatibleDC(hdc);
	char *rgbbuf = (char *)malloc(((nWidth*3+3)&~3)*nHeight+nWidth*nHeight);
	unsigned char *rgb_buf = (unsigned char*)(rgbbuf); 
	unsigned char *yuv_buf = (unsigned char*)(yuvbuf);
	
	if (rgb_buf)
	{
		ConvertYUV420PToRGB888(yuv_buf, rgb_buf, nWidth, nHeight);
	}
	HBITMAP hBmp = RGB2HBITMAP(nWidth, nHeight,rgb_buf);	
	//Select the bitmap into to the compatible device context
    HGDIOBJ hOldSel = SelectObject(hdcMem,hBmp);
    //Get the bitmap dimensions from the bitmap
    BITMAP bmp;
    GetObject(hBmp,sizeof(BITMAP),&bmp);
 
    //Copy the bitmap image from the memory DC to the screen DC

	StretchBlt(hdc, Acoordinate_x, Acoordinate_y, Awsize, Ahsize, 
		hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);//StretchBlt可将图像复制到显示区域，并按所设参数（352,288）进行缩放。

	//Restore original bitmap selection and destroy the memory DC
    SelectObject(hdcMem,hOldSel);
	DeleteDC(hdcMem);
	::DeleteObject(hBmp);
	DeleteDC(hdc);
	free(rgbbuf);
return 0;}
	}
else
	return 0;
}
