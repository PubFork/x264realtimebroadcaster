#ifndef _YUVdisplay_H
#define _YUVdisplay_H

#include "GlobalVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FORCE_WITHIN_RANGE(i, min, max) (((i)<(min))?(min):(((i)>(max))?(max):(i)))
#define yuvtorgb(y, u, v, r, g, b, i) \
	(i)=(((int)(y))*1000 + 1402*(((int)(v))-128))/1000; \
	r=FORCE_WITHIN_RANGE(i, 0, 255); \
	(i)=(((int)(y))*1000 -  344*(((int)(u))-128) - 714*(((int)(v))-127)) /1000; \
	g=FORCE_WITHIN_RANGE(i, 0, 255); \
	(i)=(((int)(y))*1000 + 1772*(((int)(u))-128))/1000; \
	b=FORCE_WITHIN_RANGE(i, 0, 255);


//void Video_displayer_set(DWORD HANDLE_DISPLAY_D, int coordinate_x_D, int coordinate_y_D, int wsize_D, int hsize_D);


void ConvertYUV420PToRGB888(const unsigned char *yuv, unsigned char *rgb, int width, int height);

//绘图函数
HBITMAP RGB2HBITMAP(int nWidth, int nHeight, unsigned char* rgb_buf);


//YUV显示主函数
int YUVdisplayer(int nWidth, int nHeight, unsigned char* yuvbuf);
int YUVdisplayerM(int nWidth, int nHeight, unsigned char* yuvbuf);

#endif