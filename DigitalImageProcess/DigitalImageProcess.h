
// DigitalImageProcess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDigitalImageProcessApp:
// �йش����ʵ�֣������ DigitalImageProcess.cpp
//

class CDigitalImageProcessApp : public CWinAppEx
{
public:
	CDigitalImageProcessApp();

	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDigitalImageProcessApp theApp;