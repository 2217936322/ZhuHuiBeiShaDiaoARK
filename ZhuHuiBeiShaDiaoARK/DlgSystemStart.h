#pragma once


// CDlgSystemStart �Ի���

class CDlgSystemStart : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSystemStart)

public:
	CDlgSystemStart();
	virtual ~CDlgSystemStart();

// �Ի�������
	enum { IDD = IDD_DLG_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
