#pragma once


// CDLGKERNEL �Ի���

class CDLGKERNEL : public CPropertyPage
{
	DECLARE_DYNAMIC(CDLGKERNEL)

public:
	CDLGKERNEL();
	virtual ~CDLGKERNEL();

// �Ի�������
	enum { IDD = IDD_DLG_KERNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
