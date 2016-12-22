#pragma once
#include "afxdlgs.h"
#include "DlgSSDT.h"
#include "DlgSSSDT.h"


// CDlgKernelHook �Ի���

class CDlgKernelHook : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgKernelHook)

public:
	CDlgKernelHook();
	virtual ~CDlgKernelHook();

// �Ի�������
	enum { IDD = IDD_DLG_KERNEL_HOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CPropertySheet		m_Sheet_KernelHook;
	CDlgSSDT			m_DlgSSDT;
	CDlgSSSDT			m_DlgSSSDT;
	virtual BOOL OnInitDialog();
	afx_msg void UloadDelFile();
};
