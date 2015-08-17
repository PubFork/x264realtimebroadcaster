
#ifndef _VideoSample_H
#define _VideoSample_H


#include "GlobalVariable.h"
#include"windows.h"
#include"vfw.h"
#pragma comment( lib, "vfw32.lib" )
#include<stdio.h>
#include "yuvdisplay.h"

#define rgbtoyuv_y(r, g, b, y) (y)=( 2990*(r) + 5870*(g) + 1140*(b))/10000
#define rgbtoyuv_u(r, g, b, u) (u)=(-1687*(r) - 3313*(g) + 5000*(b))/10000+128
#define rgbtoyuv_v(r, g, b, v) (v)=( 5000*(r) - 4187*(g) -  813*(b))/10000+128


//视频捕捉分辨率
//#define WIDTH 320
//#define HEIGHT 240

static unsigned int WIDTH;
static unsigned int HEIGHT;

//生成YUV分辨率
#define CWIDTH 176
#define CHEIGHT 144


LRESULT CALLBACK capVideoStreamCallback(HWND hWnd, LPVIDEOHDR lpVHdr);


extern HINSTANCE hInstance;
extern HINSTANCE hInst;


ATOM WindowRegister(WNDPROC lpfnWndProc, LPCWSTR lpClname, HBRUSH hbrBack);


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


INT WINAPI CameraSample();


void YUY2_RGB(BYTE *pDstData, BYTE *pSrcData, int nWidth, int nHeight);


void RGBCUT(unsigned char *rgb_buf, unsigned char *rgb_buf0,int cWidth,int cHeight,int Width,int Height);


void ConvertRGBtoYUV420P(const unsigned char *rgb, unsigned char *yuv, int width, int height);


//unsigned WINAPI CameraInit(void* pvParam);
//
//unsigned WINAPI CameraSmapleOpen(void* pvParam);
//
//unsigned WINAPI CameraSampleClose(void* pvParam);




#endif
