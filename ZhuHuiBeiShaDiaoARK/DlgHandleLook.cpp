// DlgHandleLook.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DlgHandleLook.h"
#include "afxdialogex.h"
#include <process.h>

typedef LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

//������������0�±�CountNum

typedef struct _HANDLE_INFO{
	ULONG_PTR	ObjectTypeIndex;
	ULONG_PTR	HandleValue;
	ULONG_PTR	ReferenceCount;
	ULONG_PTR	GrantedAccess;
	ULONG_PTR	CountNum;
	ULONG_PTR	Object;
	WCHAR	ProcessName[256*2];
	WCHAR	TypeName[256*2];
	WCHAR	HandleName[256*2];
} HANDLE_INFO, *PHANDLE_INFO;

PHANDLE_INFO pHandleInfo = NULL;

BOOL CDlgHandleLook::ThreadEnumHandleByZwQuerySystemInformation(DWORD pid)
{
	BOOL Io_Success = FALSE;
	ULONGLONG Buff = 0;
	DWORD dwRet = 0;
	CString str;
	ULONG_PTR Count = 0;

	pHandleInfo = (PHANDLE_INFO)malloc(sizeof(HANDLE_INFO)*1024*2);
	if(pHandleInfo == NULL)
	{
		AfxMessageBox(L"�ڴ�");
		return FALSE;
	}

	memset(pHandleInfo,'\0',1024*2*sizeof(HANDLE_INFO));

	Io_Success = DeviceIoControl(g_device,IOCTL_ENUMHANDLE, &pid, sizeof(DWORD), pHandleInfo, sizeof(HANDLE_INFO)*1024*2,&dwRet,NULL);

	if(Io_Success){

		Count = pHandleInfo[0].CountNum;
		str.Format(L"Count:%d",Count);
		AfxMessageBox(str);

		for (ULONG i = 0; i < Count; i++)
		{
			//�������   �����  ���  �������  ����   ���ô��� ����
			int item = m_List_HandleLook.GetItemCount();
			m_List_HandleLook.InsertItem(item,pHandleInfo[i].TypeName);
			m_List_HandleLook.SetItemText(item,1,pHandleInfo[i].HandleName);
			str.Format(L"0x%X",pHandleInfo[i].HandleValue);
			m_List_HandleLook.SetItemText(item,2,str);
			str.Format(L"0x%llX",pHandleInfo[i].Object);
			m_List_HandleLook.SetItemText(item,3,str);
			str.Format(L"0x%x",pHandleInfo[i].GrantedAccess);
			m_List_HandleLook.SetItemText(item,4,str);
			str.Format(L"%d",pHandleInfo[i].ReferenceCount);
			m_List_HandleLook.SetItemText(item,5,str);
			m_List_HandleLook.SetItemText(item,6,pHandleInfo[i].ProcessName);
		}

		str.Format(L"[%s]Pid:%d ----HandleCount:%d",g_CurSelectImage,pid,Count);
		this->SetWindowTextW(str);

		if(pHandleInfo)
		{
			free(pHandleInfo);
			pHandleInfo = NULL;
		}
		return TRUE;
	}	
	
	if(pHandleInfo)
		{
			free(pHandleInfo);
			pHandleInfo = NULL;
		}
	return FALSE;
}

IMPLEMENT_DYNAMIC(CDlgHandleLook, CDialogEx)

CDlgHandleLook::CDlgHandleLook(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHandleLook::IDD, pParent)
{

}

CDlgHandleLook::~CDlgHandleLook()
{
}

void CDlgHandleLook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HANDLE_LOOK, m_List_HandleLook);
}


BEGIN_MESSAGE_MAP(CDlgHandleLook, CDialogEx)
END_MESSAGE_MAP()


// CDlgHandleLook ��Ϣ�������


BOOL CDlgHandleLook::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//�����й����� ������û�дﵽҪ�� �ͻ����;
	m_List_HandleLook.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES);
	m_List_HandleLook.InsertColumn(0,L"�������",LVCFMT_CENTER,80);
	m_List_HandleLook.InsertColumn(1,L"�����",0,221);
	m_List_HandleLook.InsertColumn(2,L"���",0,50);
	m_List_HandleLook.InsertColumn(3,L"�������",0,130);
	m_List_HandleLook.InsertColumn(4,L"��������",0,80);
	m_List_HandleLook.InsertColumn(5,L"���ô���",0,80);
	m_List_HandleLook.InsertColumn(6,L"��������",0,80);
	//�������   �����  ���  �������  ����   ���ô��� ����
	if(!ThreadEnumHandleByZwQuerySystemInformation(g_CurSelectPid))
		this->SetWindowTextW(L"ʧ��!");
	return TRUE;
}


