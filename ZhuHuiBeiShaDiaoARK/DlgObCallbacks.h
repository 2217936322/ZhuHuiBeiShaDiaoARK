#pragma once
#include "afxcmn.h"


// CDlgObCallbacks �Ի���

class CDlgObCallbacks : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgObCallbacks)

public:
	CDlgObCallbacks();
	virtual ~CDlgObCallbacks();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OBCALLBACKS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_obCallbacks;
	virtual BOOL OnInitDialog();
	void EnumObCallbacks();
};
