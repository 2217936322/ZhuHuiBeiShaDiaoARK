#pragma once
#include "afxcmn.h"


// CDlgObjectHook �Ի���

class CDlgObjectHook : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgObjectHook)

public:
	CDlgObjectHook();
	virtual ~CDlgObjectHook();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OBJECTLOOK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_list_object;
	void EnumObjectType();
	afx_msg void OnObject32835();
	CString GetItemInfo(int subItem);
	afx_msg void OnNMRClickListObjecthook(NMHDR *pNMHDR, LRESULT *pResult);
};
