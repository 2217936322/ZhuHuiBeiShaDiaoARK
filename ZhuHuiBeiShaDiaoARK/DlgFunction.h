#pragma once


// CDlgFunction �Ի���

class CDlgFunction : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgFunction)

public:
	CDlgFunction();
	virtual ~CDlgFunction();

// �Ի�������
	enum { IDD = IDD_DLG_FUNCTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
