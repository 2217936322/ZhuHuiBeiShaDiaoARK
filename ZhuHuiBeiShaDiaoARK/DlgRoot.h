#pragma once


// CDlgRoot �Ի���

class CDlgRoot : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgRoot)

public:
	CDlgRoot();
	virtual ~CDlgRoot();

// �Ի�������
	enum { IDD = IDD_DLG_ROOTKIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
