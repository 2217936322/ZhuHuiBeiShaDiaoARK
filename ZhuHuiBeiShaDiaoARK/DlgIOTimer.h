#pragma once
#include "afxcmn.h"


// CDlgIOTimer �Ի���

class CDlgIOTimer : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgIOTimer)

public:
	CDlgIOTimer();
	virtual ~CDlgIOTimer();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IOTIMER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_iotimer;
	virtual BOOL OnInitDialog();
	void EnumIOTimer();
};
