// DlgObCallbacks.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DlgObCallbacks.h"
#include "afxdialogex.h"

// Ob �ص���Ϣ
typedef struct _OBCALLBACKS_INFO
{
	ULONG	Count; // 0��������Ÿ���
	ULONG	ObType;// 0 ���� 1 �߳�
	ULONG64	PreCallbackAddr;
	ULONG64	PostCallbackAddr;
	ULONG64	ObHandle;
	CHAR	PreImgPath[MAX_PATH];
	CHAR	PostImgPaht[MAX_PATH];
}OBCALLBACKS_INFO, *POBCALLBACKS_INFO;

// CDlgObCallbacks �Ի���

IMPLEMENT_DYNAMIC(CDlgObCallbacks, CPropertyPage)

CDlgObCallbacks::CDlgObCallbacks()
	: CPropertyPage(IDD_DLG_OBCALLBACKS)
{

}

CDlgObCallbacks::~CDlgObCallbacks()
{
}

void CDlgObCallbacks::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBCALLBACKS, m_list_obCallbacks);
}


BEGIN_MESSAGE_MAP(CDlgObCallbacks, CPropertyPage)
END_MESSAGE_MAP()


// CDlgObCallbacks ��Ϣ�������


BOOL CDlgObCallbacks::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_list_obCallbacks.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);
	m_list_obCallbacks.InsertColumn(0, L"�ص����", LVCFMT_CENTER, 130);
	m_list_obCallbacks.InsertColumn(1, L"����", LVCFMT_CENTER, 130);
	m_list_obCallbacks.InsertColumn(2, L"·��", 0, 270);
	m_list_obCallbacks.InsertColumn(3, L"�ļ�����", LVCFMT_CENTER, 160);
	m_list_obCallbacks.InsertColumn(4, L"ObHandle", LVCFMT_CENTER, 130);
	EnumObCallbacks();
	return TRUE;
}


void CDlgObCallbacks::EnumObCallbacks()
{
	POBCALLBACKS_INFO pObCallbacksInfo = NULL;
	CString szTrim = L"\\??\\";
	CString csSysroot;
	CString str;
	WCHAR	szSysRootBuff[MAX_PATH] = { 0 };
	WCHAR	szFileInfo[_MAX_PATH] = { 0 };
	WCHAR	*szImgPath = NULL;

	GetWindowsDirectoryW(szSysRootBuff, MAX_PATH);
	csSysroot.Format(L"%s", szSysRootBuff);

	pObCallbacksInfo = (POBCALLBACKS_INFO)malloc(sizeof(OBCALLBACKS_INFO) * 100);

	if (pObCallbacksInfo == NULL)
		return;

	memset(pObCallbacksInfo, 0, sizeof(OBCALLBACKS_INFO) * 100);

	ULONG count = 0;

	if (!ARKDeviceIoControl(IOCTL_ENUMOBCALLBACKS, NULL, 0, pObCallbacksInfo, sizeof(OBCALLBACKS_INFO) * 100))
	{
		free(pObCallbacksInfo);
		pObCallbacksInfo = NULL;
		return;
	}

	ULONG item = 0;

	count = pObCallbacksInfo[0].Count;


	for (size_t i = 0; i < count; i++)
	{
		item = m_list_obCallbacks.GetItemCount();

		
		str.Format(L"0x%p", pObCallbacksInfo[i].PreCallbackAddr);
		m_list_obCallbacks.InsertItem(item, str);

		if (pObCallbacksInfo[i].ObType == 0)// ����
			m_list_obCallbacks.SetItemText(item, 1, L"ObProcess(Pre)");
		else
			m_list_obCallbacks.SetItemText(item, 1, L"ObThread(Pre)");
		
		szImgPath = charToWchar(pObCallbacksInfo[i].PreImgPath);
		str.Format(L"%s",szImgPath);
		str = str.TrimLeft(szTrim);
		str.Replace(L"SystemRoot", csSysroot);
		m_list_obCallbacks.SetItemText(item, 2, str);

		GetFileInfo(str.GetBuffer(), szFileInfo);

		str.Format(L"%s", szFileInfo);
		m_list_obCallbacks.SetItemText(item, 3, str);

		str.Format(L"0x%p", pObCallbacksInfo[i].ObHandle);
		m_list_obCallbacks.SetItemText(item, 4, str);

		free(szImgPath);
		szImgPath = NULL;
		memset(szFileInfo, 0, MAX_PATH*2);

		if (pObCallbacksInfo[i].PostCallbackAddr == 0)
			continue;

		item = m_list_obCallbacks.GetItemCount();


		str.Format(L"0x%p", pObCallbacksInfo[i].PostCallbackAddr);
		m_list_obCallbacks.InsertItem(item, str);

		if (pObCallbacksInfo[i].ObType == 0)// ����
			m_list_obCallbacks.SetItemText(item, 1, L"ObProcess(Post)");
		else
			m_list_obCallbacks.SetItemText(item, 1, L"ObThread(Post)");

		szImgPath = charToWchar(pObCallbacksInfo[i].PostImgPaht);
		str.Format(L"%s", szImgPath);
		str = str.TrimLeft(szTrim);
		str.Replace(L"SystemRoot", csSysroot);
		m_list_obCallbacks.SetItemText(item, 2, str);

		GetFileInfo(str.GetBuffer(), szFileInfo);

		str.Format(L"%s", szFileInfo);
		m_list_obCallbacks.SetItemText(item, 3, str);

		str.Format(L"0x%p", pObCallbacksInfo[i].ObHandle);
		m_list_obCallbacks.SetItemText(item, 4, str);
		free(szImgPath);
		szImgPath = NULL;
		memset(szFileInfo, 0, MAX_PATH * 2);

	}
	str.Format(L"[ObCallbacks�鿴��]��ǰϵͳ���� %d ������ص�", count);
	AfxGetMainWnd()->SetWindowTextW(str);

	free(pObCallbacksInfo);
	pObCallbacksInfo = NULL;

}
