
// ZhuHuiBeiShaDiaoARKDlg.h : ͷ�ļ�
//
#include "DLGKERNEL.h"
#include "DLGPROCESS.h"
#include "DlgDrvier.h"
#include "DlgKernelHook.h"
#include "DlgR3Hook.h"
#include "DlgNetWork.h"
#include "DlgFile.h"
#include "DlgSystemStart.h"
#include "DlgFunction.h"
#include "DlgConfig.h"
#include "DlgRoot.h"
#include "DlgAuthor.h"

#pragma once


// CZhuHuiBeiShaDiaoARKDlg �Ի���
class CZhuHuiBeiShaDiaoARKDlg : public CDialogEx
{
// ����
public:
	CZhuHuiBeiShaDiaoARKDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZHUHUIBEISHADIAOARK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CPropertySheet		m_sheet;
	CDLGPROCESS			m_DlgPro;
	CDLGKERNEL			m_DlgKernel;
	CDlgDrvier			m_DlgDriver;
	CDlgKernelHook		m_DlgKerHook;
	CDlgR3Hook			m_DlgR3Hook;
	CDlgNetWork			m_DlgNetWork;
	CDlgFile			m_DlgFile;
	CDlgSystemStart		m_DlgSystemStart;
	CDlgFunction		m_DlgFunction;
	CDlgConfig			m_DlgConfig;
	CDlgRoot			m_DlgRootFunc;
	CDlgAuthor			m_DlgAuthor;
	virtual BOOL DestroyWindow();
};
