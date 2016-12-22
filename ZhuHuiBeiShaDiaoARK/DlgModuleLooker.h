#pragma once
#include "afxcmn.h"


// CDlgModuleLooker �Ի���

class CDlgModuleLooker : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModuleLooker)

public:
	CDlgModuleLooker(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModuleLooker();

// �Ի�������
	enum { IDD = IDD_DLG_ModuleLook };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ModuleList;
	virtual BOOL OnInitDialog();
	BOOL NtPathToDosPathW(WCHAR* FullNtPath, WCHAR* FullDosPath);
	void ZwVirEnumModule(DWORD dwProcessId);
	ULONG	num;
	afx_msg void OnNMRClickModulelook(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void ReferShow();
	void UnloadDLLAndDel(DWORD dwPid, PVOID BaseAddr, bool IsDelFile,WCHAR* wFullPath);
	afx_msg void UnloadDLL();
	afx_msg void DelFileAndUnload();
	afx_msg void OnDelFile();
	afx_msg void OnRootDelFile();
	CString wCurSelectPathName;
	int nItemSelect;
	afx_msg void OnPointFile();
	CString GetCurSelectText(int subItem);
	afx_msg void OnHideModule();
	BOOL HideModule(int hideMod);
	afx_msg void OnHideModulebyMZPE();
	afx_msg void OnHideModuleByAllHeader();
	afx_msg void OnWeiZhuangModule();
};
