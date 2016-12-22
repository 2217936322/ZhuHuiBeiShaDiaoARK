// DlgDrvier.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DlgDrvier.h"
#include "afxdialogex.h"
#include "DlgDisPatch.h"

// ����ģ���ַ��Ϣ
typedef struct _DRIVER_INFO
{
	// ����˳��
	ULONG index;
	// ��С
	ULONG	size;
	// ���ش���
	ULONG LoadCount;
    // ����ַ
	ULONG_PTR Base;
    // ��ڵ�ַ
    ULONG_PTR EntryPoint;
    // ��������
	ULONG_PTR DriverObject;
	// �豸����
	ULONG_PTR DeviceObject;
	// ������
	WCHAR ServerName[MAX_PATH];
	// ������
	WCHAR DriverName[MAX_PATH];
    // ģ��·��
    WCHAR ImagePath[MAX_PATH];
    // ģ������
    ULONG Count;
} DRIVER_INFO, *PDRIVER_INFO;

PDRIVER_INFO pDriverInfo = NULL;

// CDlgDrvier �Ի���

IMPLEMENT_DYNAMIC(CDlgDrvier, CPropertyPage)

CDlgDrvier::CDlgDrvier()
	: CPropertyPage(CDlgDrvier::IDD)
{

}

CDlgDrvier::~CDlgDrvier()
{
}

void CDlgDrvier::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DRIVER, m_list_dirver);
}


BEGIN_MESSAGE_MAP(CDlgDrvier, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DRIVER, &CDlgDrvier::OnNMRClickListDriver)
	ON_COMMAND(ID_DRIVER_32817, &CDlgDrvier::OnDriverRef)
	ON_COMMAND(ID_DRIVER_32826, &CDlgDrvier::OnDriverCopyDriverName)
	ON_COMMAND(ID_DRIVER_32827, &CDlgDrvier::OnDriverCopyDriverPath)
	ON_COMMAND(ID_DRIVER_32828, &CDlgDrvier::OnDriverCopyBase)
	ON_COMMAND(ID_DRIVER_32829, &CDlgDrvier::OnDriverCopyDriverObject)
	ON_COMMAND(ID_DRIVER_32830, &CDlgDrvier::OnDriverCopyDeviceObject)
	ON_COMMAND(ID_DRIVER_32831, &CDlgDrvier::OnDriverCopyDriverEntry)
	ON_COMMAND(ID_DRIVER_32825, &CDlgDrvier::OnDriverLookFileInfo)
	ON_COMMAND(ID_DRIVER_32834, &CDlgDrvier::OnDriverOpenPath)
	ON_COMMAND(ID_DRIVER_32832, &CDlgDrvier::OnDriverFileTrust)
	ON_COMMAND(ID_DRIVER_32824, &CDlgDrvier::OnDriverViewIrpDispatch)
END_MESSAGE_MAP()


// CDlgDrvier ��Ϣ�������


BOOL CDlgDrvier::OnInitDialog()
{
	CPropertyPage::OnInitDialog();


	m_list_dirver.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES);
	//m_list_dirver.InsertColumn(0,L"index",0,50);
	m_list_dirver.InsertColumn(0,L"������",0,90);
	m_list_dirver.InsertColumn(1,L"����ַ",0,90);
	m_list_dirver.InsertColumn(2,L"��С",0,60);
	m_list_dirver.InsertColumn(3,L"��ڵ�ַ",0,90);
	m_list_dirver.InsertColumn(4,L"��������",0,90);
	m_list_dirver.InsertColumn(5,L"�豸����",0,90);
	m_list_dirver.InsertColumn(6,L"���ش���",0,40);
	m_list_dirver.InsertColumn(7,L"������",0,70);
	m_list_dirver.InsertColumn(8,L"·��",0,200);
	m_list_dirver.InsertColumn(9,L"�ļ�����",0,160);
	EnumDriver();
	return TRUE;
}


void CDlgDrvier::EnumDriver(void)
{
	CString str;
	CString szTrim = L"\\??\\";
	WCHAR	szSysRootBuff[MAX_PATH] = {0};
	WCHAR	szFileInfo[_MAX_PATH] = {0};
	CString csSysroot;
	ULONG	uStartIndex = 0;
	GetWindowsDirectoryW(szSysRootBuff, MAX_PATH);
	csSysroot.Format(L"%s",szSysRootBuff);

	pDriverInfo = (PDRIVER_INFO)malloc(sizeof(DRIVER_INFO)*500);
	if(pDriverInfo == NULL)
		return;

	if(!ARKDeviceIoControl(IOCTL_ENUMDRIVER, &pDriverInfo,8,pDriverInfo,sizeof(DRIVER_INFO)*500)){
		free(pDriverInfo);
		return;
	}
	int nItem = 0;
	ULONG count = pDriverInfo[0].Count;
	for (ULONG k = 0; k < count; k++)
	{
		if(wcsstr(pDriverInfo[k].DriverName, L"ntos"))
		{
			uStartIndex = k;
			break;
		}
		if(k > 20)
			break;
	}

	for (ULONG i = uStartIndex; i < count; i++)
	{
		nItem = m_list_dirver.GetItemCount();
		str.Format(L"%s",pDriverInfo[i].DriverName);
		m_list_dirver.InsertItem(nItem, str);

		str.Format(L"0x%p", pDriverInfo[i].Base);
		m_list_dirver.SetItemText(nItem, 1, str);

		str.Format(L"0x%X", pDriverInfo[i].size);
		m_list_dirver.SetItemText(nItem, 2, str);

		if(pDriverInfo[i].EntryPoint != 0)
			str.Format(L"0x%p", pDriverInfo[i].EntryPoint);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 3, str);

		if(pDriverInfo[i].DriverObject != 0)
			str.Format(L"0x%p", pDriverInfo[i].DriverObject);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 4, str);

		if(pDriverInfo[i].DeviceObject != 0)
			str.Format(L"0x%p", pDriverInfo[i].DeviceObject);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 5, str);

		str.Format(L"%d", pDriverInfo[i].LoadCount);
		m_list_dirver.SetItemText(nItem, 6, str);

		str.Format(L"%s", pDriverInfo[i].ServerName);
		m_list_dirver.SetItemText(nItem, 7, str);

		str.Format(L"%s", pDriverInfo[i].ImagePath);
		str = str.TrimLeft(szTrim);
		str.Replace(L"SystemRoot", csSysroot);
		m_list_dirver.SetItemText(nItem, 8, str);

		GetFileInfo(str.GetBuffer(), szFileInfo);
		str.Format(L"%s", szFileInfo);
		m_list_dirver.SetItemText(nItem, 9,str);
		
		memset(szFileInfo, 0, MAX_PATH);
	}

	for (ULONG t = 0; t < uStartIndex; t++)
	{
		nItem = m_list_dirver.GetItemCount();
		str.Format(L"%s",pDriverInfo[t].DriverName);
		m_list_dirver.InsertItem(nItem, str);

		str.Format(L"0x%p", pDriverInfo[t].Base);
		m_list_dirver.SetItemText(nItem, 1, str);

		str.Format(L"0x%X", pDriverInfo[t].size);
		m_list_dirver.SetItemText(nItem, 2, str);

		if(pDriverInfo[t].EntryPoint != 0)
			str.Format(L"0x%p", pDriverInfo[t].EntryPoint);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 3, str);

		if(pDriverInfo[t].DriverObject != 0)
			str.Format(L"0x%p", pDriverInfo[t].DriverObject);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 4, str);

		if(pDriverInfo[t].DeviceObject != 0)
			str.Format(L"0x%p", pDriverInfo[t].DeviceObject);
		else
			str.Format(L"");
		m_list_dirver.SetItemText(nItem, 5, str);

		str.Format(L"%d", pDriverInfo[t].LoadCount);
		m_list_dirver.SetItemText(nItem, 6, str);

		str.Format(L"%s", pDriverInfo[t].ServerName);
		m_list_dirver.SetItemText(nItem, 7, str);

		str.Format(L"%s", pDriverInfo[t].ImagePath);
		str = str.TrimLeft(szTrim);
		str.Replace(L"SystemRoot", csSysroot);
		m_list_dirver.SetItemText(nItem, 8, str);

		GetFileInfo(str.GetBuffer(), szFileInfo);
		str.Format(L"%s", szFileInfo);
		m_list_dirver.SetItemText(nItem, 9,str);
		
		memset(szFileInfo, 0, MAX_PATH);
		
	}
	
	
	free(pDriverInfo);

	str.Format(L"��ᱻɱ��:[����������]��ǰϵͳ���� %d ������ģ��", count);

	AfxGetMainWnd()->SetWindowTextW(str);
}


void CDlgDrvier::OnNMRClickListDriver(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int istat = m_list_dirver.GetSelectionMark();//��istat��ŵ�ǰѡ�����ǵڼ���
	//m_strSelectedTopic = m_list_dirver.GetItemText(istat,1);//��ȡ��ǰ���е����ݣ�0�����ǵ�0��

	POINT point;
	HMENU hMenu, hSubMenu;
	GetCursorPos(&point); //���λ��
	hMenu = LoadMenu(NULL, 
		MAKEINTRESOURCE(IDR_MENU_DRIVER)); // ���ز˵�
	hSubMenu = GetSubMenu(hMenu, 0);//�õ��Ӳ˵�(��Ϊ����ʽ�˵����Ӳ˵�)

	SetMenuDefaultItem(hSubMenu, -1, FALSE);//����ȱʡ�˵���,-1Ϊ��ȱʡ��
	SetForegroundWindow(); // ����ڲ���ǰ

	TrackPopupMenu(hSubMenu, 0, 
		point.x, point.y, 0, m_hWnd, NULL);
	*pResult = 0;
}


void CDlgDrvier::OnDriverRef()
{
	m_list_dirver.DeleteAllItems();
	EnumDriver();
}


void CDlgDrvier::OnDriverCopyDriverName()
{
	CopyInfoToBoard(0);
}


void CDlgDrvier::OnDriverCopyDriverPath()
{
	CopyInfoToBoard(8);
}


void CDlgDrvier::OnDriverCopyBase()
{
	CopyInfoToBoard(1);
}


void CDlgDrvier::OnDriverCopyDriverObject()
{
	CopyInfoToBoard(4);
}


void CDlgDrvier::OnDriverCopyDeviceObject()
{
	CopyInfoToBoard(5);
}


void CDlgDrvier::OnDriverCopyDriverEntry()
{
	CopyInfoToBoard(3);
}


CString CDlgDrvier::GetItemInfo(int subItem)
{
	nItemSelct = m_list_dirver.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelct!=-1)
		return m_list_dirver.GetItemText(nItemSelct,subItem);
	return NULL;
}


void CDlgDrvier::CopyInfoToBoard(int subItem)
{
	CString str;
	str = GetItemInfo(subItem);
	CopyToClipboard(m_hWnd, str);
}


void CDlgDrvier::OnDriverLookFileInfo()
{
	CString str;
	str = GetItemInfo(8);
	LookFileAttrubute(str.GetBuffer());
}


void CDlgDrvier::OnDriverOpenPath()
{
	CString str;
	str = GetItemInfo(8);

	HANDLE hFile = CreateFileW( str.GetBuffer(), GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL );
    if ( INVALID_HANDLE_VALUE == hFile )
    {
		AfxMessageBox(L"�ļ�������");
		return;
    }

	str = L"/select," + str;
	ShellExecute(0,L"open",L"Explorer.exe",str,NULL,SW_NORMAL);
}


void CDlgDrvier::OnDriverFileTrust()
{
	CString FileName;
	FileName = GetItemInfo(8);
	CheckFileTrust(FileName.GetBuffer(),FALSE,0,FALSE);
}


void CDlgDrvier::OnDriverViewIrpDispatch()
{
	g_DriverName = GetItemInfo(0);
	CDlgDisPatch DlgDisPathView;
	DlgDisPathView.DoModal();
}



