#pragma once
#include "afxcmn.h"


// CDlgFilterDriver �Ի���

class CDlgFilterDriver : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgFilterDriver)

public:
	CDlgFilterDriver();
	virtual ~CDlgFilterDriver();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FILTER_DRIVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_filterDrv;
	virtual BOOL OnInitDialog();
	void EnumFilterDriver();
};
