#pragma once


// CDlgConfig �Ի���

class CDlgConfig : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig();
	virtual ~CDlgConfig();

// �Ի�������
	enum { IDD = IDD_DLG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
