#pragma once
#include "afxcmn.h"


// CDlgDpcTimer �Ի���

class CDlgDpcTimer : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgDpcTimer)

public:
	CDlgDpcTimer();
	virtual ~CDlgDpcTimer();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DPCTIMER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_DpcTimer;
	virtual BOOL OnInitDialog();
	void EnumDpcTimer();
};
