// begin1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#include <comdef.h>
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cbegin1App:
// �йش����ʵ�֣������ begin1.cpp
//

class Cbegin1App : public CWinApp
{
public:
	Cbegin1App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cbegin1App theApp;
#pragma comment(lib, "comsupp.lib")