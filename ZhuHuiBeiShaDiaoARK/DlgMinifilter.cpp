// DlgMinifilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DlgMinifilter.h"
#include "afxdialogex.h"


typedef struct _MINIFILTER_INFO
{
	ULONG	FltNum;	//�������ĸ���
	ULONG	IrpCount; // Irp������
	ULONG	Irp;
	ULONG64	Object;
	ULONG64	PreFunc;
	ULONG64	PostFunc;
	CHAR	PreImgPath[MAX_PATH];
	CHAR	PostImgPath[MAX_PATH];
}MINIFILTER_INFO, *PMINIFILTER_INFO;

// CDlgMinifilter �Ի���

IMPLEMENT_DYNAMIC(CDlgMinifilter, CPropertyPage)

CDlgMinifilter::CDlgMinifilter()
	: CPropertyPage(IDD_DLG_MINIFILTER)
{

}

CDlgMinifilter::~CDlgMinifilter()
{
}

void CDlgMinifilter::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MINIFILTER, m_list_minifilter);
}


BEGIN_MESSAGE_MAP(CDlgMinifilter, CPropertyPage)
END_MESSAGE_MAP()


// CDlgMinifilter ��Ϣ�������


BOOL CDlgMinifilter::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_list_minifilter.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);
	m_list_minifilter.InsertColumn(0, L"FltObject", 0, 75);
	m_list_minifilter.InsertColumn(1, L"Irp", 0, 150);
	m_list_minifilter.InsertColumn(2, L"Pre", 0, 75);
	m_list_minifilter.InsertColumn(3, L"Post",0, 75);
	m_list_minifilter.InsertColumn(4, L"Pre�ļ�����", 0, 100);
	m_list_minifilter.InsertColumn(5, L"Pre����ģ��", 0, 200);
	m_list_minifilter.InsertColumn(6, L"Post�ļ�����", 0, 140);
	m_list_minifilter.InsertColumn(7, L"Post����ģ��", 0, 200);
	
	EnumMinifilter();
	return TRUE;
}


void CDlgMinifilter::EnumMinifilter()
{
	PMINIFILTER_INFO pInfo = NULL;
	CString szTrim = L"\\??\\";
	CString csSysroot;
	CString str;
	WCHAR	szSysRootBuff[MAX_PATH] = { 0 };
	WCHAR	szFileInfo[_MAX_PATH] = { 0 };

	GetWindowsDirectoryW(szSysRootBuff, MAX_PATH);
	csSysroot.Format(L"%s", szSysRootBuff);

	pInfo = (PMINIFILTER_INFO)malloc(sizeof(MINIFILTER_INFO)*1000);

	if (pInfo == NULL)
		return;

	RtlZeroMemory(pInfo, sizeof(MINIFILTER_INFO)*1000);

	if (!ARKDeviceIoControl(IOCTL_ENUMMINIFILTER, NULL, 0, pInfo, sizeof(MINIFILTER_INFO) * 1000))
	{
		free(pInfo);
		pInfo = NULL;
		return;
	}

	ULONG IrpCount = 0;
	ULONG item = 0;
	IrpCount = pInfo[0].IrpCount;

	for (size_t i = 0; i < IrpCount; i++)
	{
		item = m_list_minifilter.GetItemCount();

		str.Format(L"0x%p", pInfo[i].Object);
		m_list_minifilter.InsertItem(item, str);

		str.Format(L"%S", MajorFun[pInfo[i].Irp]);
		m_list_minifilter.SetItemText(item, 1, str);

		if (pInfo[i].PreFunc != 0)
		{
			str.Format(L"0x%p", pInfo[i].PreFunc);
			m_list_minifilter.SetItemText(item, 2, str);

			str.Format(L"%S", pInfo[i].PreImgPath);
			str = str.TrimLeft(szTrim);
			str.Replace(L"SystemRoot", csSysroot);
			m_list_minifilter.SetItemText(item, 5, str);

			GetFileInfo(str.GetBuffer(), szFileInfo);
			str.Format(L"%s", szFileInfo);
			m_list_minifilter.SetItemText(item,4, str);
		}

		memset(szFileInfo, 0, MAX_PATH * 2);

		if (pInfo[i].PostFunc != 0)
		{
			str.Format(L"0x%p", pInfo[i].PostFunc);
			m_list_minifilter.SetItemText(item, 3, str);

			str.Format(L"%S", pInfo[i].PreImgPath);
			str = str.TrimLeft(szTrim);
			str.Replace(L"SystemRoot", csSysroot);
			m_list_minifilter.SetItemText(item, 7, str);

			GetFileInfo(str.GetBuffer(), szFileInfo);
			str.Format(L"%s", szFileInfo);
			m_list_minifilter.SetItemText(item, 6, str);
		}

		memset(szFileInfo, 0, MAX_PATH * 2);
	}

	str.Format(L"��ǰϵͳMinifilter����:%d	Irp:%d",pInfo[0].FltNum, IrpCount);
	AfxGetMainWnd()->SetWindowTextW(str);

	free(pInfo);
	pInfo = NULL;
}
