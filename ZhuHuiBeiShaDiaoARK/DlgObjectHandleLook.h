#pragma once
#include "afxcmn.h"


// CDlgObjectHandleLook �Ի���

class CDlgObjectHandleLook : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgObjectHandleLook)

public:
	CDlgObjectHandleLook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgObjectHandleLook();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OBJECT_PRODU_LOOK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_objectHandle;
	void CDlgObjectHandleLook::InsertObjeceHandleInfo(CString HandleName, ULONG64 addr, CHAR* ImgPath);
	virtual BOOL OnInitDialog();
	void GetObjectHandle(UCHAR Index);
};
