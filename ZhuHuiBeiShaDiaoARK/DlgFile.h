#pragma once


// CDlgFile �Ի���

class CDlgFile : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgFile)

public:
	CDlgFile();
	virtual ~CDlgFile();

// �Ի�������
	enum { IDD = IDD_DLG_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
