#pragma once
#include "afxcmn.h"


// CDlgHandleLook �Ի���

class CDlgHandleLook : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHandleLook)

public:
	CDlgHandleLook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHandleLook();

// �Ի�������
	enum { IDD = IDD_DLG_HANDLELOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List_HandleLook;
	BOOL ThreadEnumHandleByZwQuerySystemInformation(DWORD pid);
};
