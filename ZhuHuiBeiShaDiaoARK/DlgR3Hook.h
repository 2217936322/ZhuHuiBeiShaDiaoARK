#pragma once


// CDlgR3Hook �Ի���

class CDlgR3Hook : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgR3Hook)

public:
	CDlgR3Hook();
	virtual ~CDlgR3Hook();

// �Ի�������
	enum { IDD = IDD_DLG_RING3_HOOK1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
