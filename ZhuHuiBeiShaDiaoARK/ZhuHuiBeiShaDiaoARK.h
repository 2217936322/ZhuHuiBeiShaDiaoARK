
// ZhuHuiBeiShaDiaoARK.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CZhuHuiBeiShaDiaoARKApp:
// �йش����ʵ�֣������ ZhuHuiBeiShaDiaoARK.cpp
//

class CZhuHuiBeiShaDiaoARKApp : public CWinApp
{
public:
	CZhuHuiBeiShaDiaoARKApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZhuHuiBeiShaDiaoARKApp theApp;