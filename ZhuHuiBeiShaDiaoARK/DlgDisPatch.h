#pragma once
#include "afxcmn.h"

// CDlgDisPatch �Ի���

class CDlgDisPatch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisPatch)

public:
	CDlgDisPatch(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDisPatch();

// �Ի�������
	enum { IDD = IDD_DLG_DISPATCHLOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_DisPatch;
	virtual BOOL OnInitDialog();
	BOOL EnumIrpDisPatch(PWCHAR DriverName);
};
