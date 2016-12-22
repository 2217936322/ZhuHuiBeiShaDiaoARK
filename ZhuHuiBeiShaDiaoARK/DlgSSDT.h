#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"


// CDlgSSDT �Ի���

class CDlgSSDT : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSSDT)

public:
	CDlgSSDT();
	virtual ~CDlgSSDT();

// �Ի�������
	enum { IDD = IDD_DIALOG_SSDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CMyListCtrl m_list_ssdt;
	CListCtrl m_list_ssdt;
	virtual BOOL OnInitDialog();
	static DWORD m_SortColum;  
	static BOOL m_bAs; 
	static int CALLBACK MyListCompar(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void EnumSSDT(void);
	afx_msg void MyListCompar(NMHDR *pNMHDR, LRESULT *pResult);
};
