#pragma once
#include "afxcmn.h"


// CDlgSystemCall �Ի���

class CDlgSystemCall : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSystemCall)

public:
	CDlgSystemCall();
	virtual ~CDlgSystemCall();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CALLBACKS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_syscallbacks;
	virtual BOOL OnInitDialog();
	void EnumSystemCallbacks();
};
