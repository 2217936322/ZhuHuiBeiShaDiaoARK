#pragma once
#include "afxcmn.h"


// CDlgMinifilter �Ի���

class CDlgMinifilter : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgMinifilter)

public:
	CDlgMinifilter();
	virtual ~CDlgMinifilter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MINIFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_minifilter;
	virtual BOOL OnInitDialog();
	void EnumMinifilter();
};
