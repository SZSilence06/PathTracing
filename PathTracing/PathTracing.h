
// PathTracing.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPathTracingApp: 
// �йش����ʵ�֣������ PathTracing.cpp
//

class CPathTracingApp : public CWinApp
{
public:
	CPathTracingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPathTracingApp theApp;