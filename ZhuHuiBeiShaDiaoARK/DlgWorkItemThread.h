#pragma once
#include "afxcmn.h"


// CDlgWorkItemThread �Ի���

class CDlgWorkItemThread : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgWorkItemThread)

public:
	CDlgWorkItemThread();
	virtual ~CDlgWorkItemThread();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WORKITEMTHREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_workitem;
	virtual BOOL OnInitDialog();
	void EnumWorkItemThread();
};
