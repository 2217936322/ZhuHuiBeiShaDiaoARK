#pragma once
#include "afxcmn.h"


// CDlgSSSDT �Ի���

class CDlgSSSDT : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSSSDT)

public:
	CDlgSSSDT();
	virtual ~CDlgSSSDT();

// �Ի�������
	enum { IDD = IDD_DIALOG_SSSDT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_sssdt;
	virtual BOOL OnInitDialog();
	void EnumSssdt();
	BOOLEAN GetSssdtOrgAddress();
	ULONG64 GetWin32kBase();
};
