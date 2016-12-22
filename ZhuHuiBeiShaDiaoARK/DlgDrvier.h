#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CDlgDrvier �Ի���

class CDlgDrvier : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgDrvier)

public:
	CDlgDrvier();
	virtual ~CDlgDrvier();

// �Ի�������
	enum { IDD = IDD_DLG_DRVIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int nItemSelct;
	//CListCtrl m_list_dirver;
	CMyListCtrl m_list_dirver;

	virtual BOOL OnInitDialog();
	void EnumDriver(void);
	afx_msg void OnNMRClickListDriver(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDriverRef();
	afx_msg void OnDriverCopyDriverName();
	afx_msg void OnDriverCopyDriverPath();
	afx_msg void OnDriverCopyBase();
	afx_msg void OnDriverCopyDriverObject();
	afx_msg void OnDriverCopyDeviceObject();
	afx_msg void OnDriverCopyDriverEntry();
	CString GetItemInfo(int subItem);
	void CopyInfoToBoard(int subItem);
	afx_msg void OnDriverLookFileInfo();
	afx_msg void OnDriverOpenPath();
	afx_msg void OnDriverFileTrust();
	afx_msg void OnDriverViewIrpDispatch();
};
