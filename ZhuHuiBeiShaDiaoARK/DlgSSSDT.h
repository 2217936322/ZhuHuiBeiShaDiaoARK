#pragma once


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
};
