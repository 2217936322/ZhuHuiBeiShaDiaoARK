#pragma once
#include "afxcmn.h"


// CDlgThreadLooker �Ի���

class CDlgThreadLooker : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThreadLooker)

public:
	CDlgThreadLooker(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgThreadLooker();

// �Ի�������
	enum { IDD = IDD_DLG_THREAD_LOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_Looker_Thread;
	virtual BOOL OnInitDialog();
	void EnumThreadByPid(DWORD pid);
};
