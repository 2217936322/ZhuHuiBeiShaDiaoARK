// DlgModuleLooker.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DlgModuleLooker.h"
#include "afxdialogex.h"
#include <windows.h>
#include "DLGPROCESS.h"


// CDlgModuleLooker �Ի���

IMPLEMENT_DYNAMIC(CDlgModuleLooker, CDialogEx)

CDlgModuleLooker::CDlgModuleLooker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModuleLooker::IDD, pParent)
	, wCurSelectPathName(_T(""))
	, nItemSelect(0)
{

}

CDlgModuleLooker::~CDlgModuleLooker()
{
}

void CDlgModuleLooker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ModuleLook, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CDlgModuleLooker, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_ModuleLook, &CDlgModuleLooker::OnNMRClickModulelook)
	ON_COMMAND(ID_32798, &CDlgModuleLooker::ReferShow)
	ON_COMMAND(ID_Menu32799, &CDlgModuleLooker::UnloadDLL)
	ON_COMMAND(ID_32800, &CDlgModuleLooker::DelFileAndUnload)
	ON_COMMAND(ID_32802, &CDlgModuleLooker::OnDelFile)
	ON_COMMAND(ID_32803, &CDlgModuleLooker::OnRootDelFile)
	ON_COMMAND(ID_Menu32805, &CDlgModuleLooker::OnPointFile)
	//ON_COMMAND(ID_Menu32806, &CDlgModuleLooker::OnHideModule)
	ON_COMMAND(ID_32811, &CDlgModuleLooker::OnHideModulebyMZPE)
	ON_COMMAND(ID_32812, &CDlgModuleLooker::OnHideModuleByAllHeader)
	ON_COMMAND(ID_32807, &CDlgModuleLooker::OnWeiZhuangModule)
END_MESSAGE_MAP()


// CDlgModuleLooker ��Ϣ�������


BOOL CDlgModuleLooker::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	num =0;
	CString str;
	m_ModuleList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES);

	m_ModuleList.InsertColumn(0,L"ģ������",0,250);
	m_ModuleList.InsertColumn(1,L"��ַ",0,130);
	m_ModuleList.InsertColumn(2,L"�ļ�����",0,200);

	ZwVirEnumModule(g_CurSelectPid);
	str.Format(L"���̹���%d��ģ��",num);

	this->SetWindowTextW(g_CurSelectImage+str);
	return TRUE;
}


BOOL CDlgModuleLooker::NtPathToDosPathW(WCHAR* FullNtPath, WCHAR* FullDosPath)
{
		WCHAR DosDevice[4]= {0};       //dos�豸����󳤶�Ϊ4
        WCHAR NtPath[64]= {0};       //nt�豸����󳤶�Ϊ64
        WCHAR *RetStr=NULL;
        size_t NtPathLen=0;
		short i = 0;
        if (!FullNtPath || !FullDosPath)
        {
                return FALSE;
        }
        for(i=65; i<26+65; i++)
        {
                DosDevice[0]=i;
                DosDevice[1]=L':';
                if(QueryDosDeviceW(DosDevice,NtPath,64))
                {
                        if (NtPath)
                        {
                                NtPathLen=wcslen(NtPath);
                                if (!_wcsnicmp(NtPath,FullNtPath,NtPathLen))
                                {
                                        wcscpy(FullDosPath,DosDevice);
                                        wcscat(FullDosPath,FullNtPath+NtPathLen);
                                        return TRUE;
                                }
                        }
                }
        }
        return FALSE;
}


void CDlgModuleLooker::ZwVirEnumModule(DWORD dwProcessId)
{
   DWORD64 dwStartAddr = 0x00000000;
   BYTE szBuffer[MAX_PATH * 2 + 4] = {0};
   WCHAR szModuleName[MAX_PATH] = {0};
   WCHAR szPathName[MAX_PATH] = {0};
   MEMORY_BASIC_INFORMATION mbi;
   PUNICODE_STRING usSectionName;   
   ZWQUERYVIRTUALMEMORY fnZwQueryVirtualMemory;
   BOOL modulex64 = FALSE;
   HANDLE hProcess =NULL;
   ULONG	dwRetVal=0;
   ULONG   item = 0;
   CString str;
   WCHAR* pFileInfo = (WCHAR*)malloc(512);
   CDLGPROCESS tem;
   memset(pFileInfo,'\0',512);
   
   hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessId);
 
   if (hProcess == NULL)
   {
      //wprintf(L"Open Process %d Error\n", dwProcessId);
	   this->SetWindowTextW(L"�򿪽���ʧ��");
      return;
   }

   dwStartAddr = 0x0000000000000000;
 
   fnZwQueryVirtualMemory = (ZWQUERYVIRTUALMEMORY)GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryVirtualMemory" );
 
   if(fnZwQueryVirtualMemory)
   {
      do
      {
         if (fnZwQueryVirtualMemory(
            hProcess,
            (PVOID64)dwStartAddr,
            MemoryBasicInformation,
            &mbi,
            sizeof(mbi),
            0) >= 0 )
         {
            if(mbi.Type == MEM_IMAGE)
            {
                if (fnZwQueryVirtualMemory(
                   hProcess,
                   (PVOID64)dwStartAddr,
                   MemorySectionName,
                   szBuffer,
                   sizeof(szBuffer),
                   0) >= 0 )
                {
                   usSectionName = (PUNICODE_STRING)szBuffer;
                   if( _wcsnicmp(szModuleName, usSectionName->Buffer, usSectionName->Length / sizeof(WCHAR)) )
                   {
					  wcsncpy(szModuleName,usSectionName->Buffer,usSectionName->Length/sizeof(WCHAR));
                      szModuleName[usSectionName->Length / sizeof(WCHAR)] = UNICODE_NULL;
					  NtPathToDosPathW(szModuleName,szPathName);
					  tem.GetFileInfo(szPathName,pFileInfo);
					  str.Format(L"0x%p",dwStartAddr);
					  item = m_ModuleList.GetItemCount();
					  m_ModuleList.InsertItem(item,szPathName);
					  m_ModuleList.SetItemText(item,1,str);
					  m_ModuleList.SetItemText(item,2,pFileInfo);
					  memset(pFileInfo,'\0',512);
					  num++;
                  }
                }
            }
 
         }
         // ������ַ,��ʼ��һ�ֲ�ѯ!
		 dwStartAddr += (ULONGLONG)0x1000;
		 if(!modulex64){
			 if(dwStartAddr>0x0000000200000000)
			 {
				 modulex64 = TRUE;
				 dwStartAddr = 0x000007fe00000000;
			 }
		 }
      }while( dwStartAddr < 0x000007ff00000000 );
   }
 
   CloseHandle(hProcess);
   free(pFileInfo);
}


void CDlgModuleLooker::OnNMRClickModulelook(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		*pResult = 0;
	
	CMenu menu;
	menu.LoadMenuW(IDR_MENU2);
	CMenu *pM = menu.GetSubMenu(0);
	
	CPoint pt;
	GetCursorPos(&pt);

	////�ж��Ƿ�ѡ��
	//int nCursel = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	//if(nCursel==-1)
	//{
	//	//���ò鿴��ϢΪ����
	//	menu.EnableMenuItem(ID_SHOWWINDOWS_32772,MF_GRAYED);
	//}

	pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}


void CDlgModuleLooker::ReferShow()//ˢ��
{
	CString str;
	num = 0;
	m_ModuleList.DeleteAllItems();
	ZwVirEnumModule(g_CurSelectPid);
	str.Format(L"���̹���%d��ģ��",num);
	this->SetWindowTextW(g_CurSelectImage+str);
}


void CDlgModuleLooker::UnloadDLLAndDel(DWORD dwPid, PVOID BaseAddr, bool IsDelFile,WCHAR* wFullPath)
{
	CDLGPROCESS tem;
	CString str;
	long zwret=0xC0000001; //STATUS_UNSUCCESSFUL
	HANDLE hp=tem.NtdllOpenProcess(PROCESS_ALL_ACCESS,0,dwPid);
	if (hp>0)
		zwret=ZwUnmapViewOfSection(hp,(PVOID)BaseAddr);
	if (IsDelFile)
	{
		DeleteFileW(wFullPath);
	}
	if(zwret!=0)
	{
		str.Format(L"ж��ʧ��!Error Code: 0x%x",zwret);
		AfxMessageBox(str);
		return;
	}
	AfxMessageBox(L"ж�سɹ�!");
}


void CDlgModuleLooker::UnloadDLL()
{
	ULONG64 DllAddr = 0;
	CString wBassAddr = 0;
	nItemSelect = m_ModuleList.GetNextItem(-1,LVIS_SELECTED); 
	if(nItemSelect!=-1)
		wBassAddr = m_ModuleList.GetItemText(nItemSelect,1);
	AfxMessageBox(wBassAddr);
	DllAddr = _tcstoui64(wBassAddr,NULL,16);
	UnloadDLLAndDel(g_CurSelectPid,(PVOID)DllAddr,FALSE,NULL);
}


void CDlgModuleLooker::DelFileAndUnload()
{
	ULONG64 DllAddr = 0;
	CString wBassAddr = 0,wPathName = 0;
	nItemSelect = m_ModuleList.GetNextItem(-1,LVIS_SELECTED); 
	if(nItemSelect!=-1){
		wBassAddr = m_ModuleList.GetItemText(nItemSelect,1);
		wPathName = m_ModuleList.GetItemText(nItemSelect,0);
	}
	DllAddr = (ULONG64)_tcstoui64(wBassAddr,NULL,16);;
	UnloadDLLAndDel(g_CurSelectPid,(PVOID)DllAddr,TRUE,wPathName.GetBuffer());
}


void CDlgModuleLooker::OnDelFile()
{
	CString wPathName = 0;
	nItemSelect = m_ModuleList.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelect!=-1)
		wPathName = m_ModuleList.GetItemText(nItemSelect,0);
	if(!DeleteFile(wPathName))
		AfxMessageBox(L"ɾ��ʧ��!�볢��ʹ��\"������ɾ��\"����!");
}

//������ɾ��
void CDlgModuleLooker::OnRootDelFile()
{
	wCurSelectPathName = GetCurSelectText(0);
	if(MoveFileExW(wCurSelectPathName,NULL,MOVEFILE_DELAY_UNTIL_REBOOT))
		AfxMessageBox(L"��ӳɹ�������ȡ�������и���һ�ݣ�");
}

//��λ�ļ�
void CDlgModuleLooker::OnPointFile()
{
	wCurSelectPathName = L"/select," + GetCurSelectText(0);
	ShellExecute(0,L"open",L"Explorer.exe",wCurSelectPathName,NULL,SW_NORMAL);
}

//��ȡ����ѡ������Ϣ
CString CDlgModuleLooker::GetCurSelectText(int subItem)
{
	nItemSelect = m_ModuleList.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelect!=-1)
		return m_ModuleList.GetItemText(nItemSelect,subItem);
	return L"C:\\Windows\\System32\\alg.exe";//ʧ�ܾͷ������
}


//����0 ��ĨMZ PE��־  1 Ĩ����ͷ��1000�ֽ�
BOOL CDlgModuleLooker::HideModule(int hideMod)
{
	//��������
	//1���ں��в��� ��PE�ṹȥ���� ��Ҫע��PE�ṹ��ͬ���ܲ�ͬ�����  ��32λģ����64λģ��
	//2ֱ�Ӳ���CR3ǿ�ƶ�д�ڴ� ��ģ�鿪ʼ��ַ��ǰ1000�ֽ� ��0 �����������λMZ PE��־
	//2���Բ���2�ַ�ʽ;1��WriteProcessMemory 2������ν��Cr3ǿ���л���ַ����д�ڴ�
	return 0;
}

//ĨMZPZ
void CDlgModuleLooker::OnHideModulebyMZPE()
{
	// TODO: �ڴ���������������
}

//Ĩ1000�ֽ�
void CDlgModuleLooker::OnHideModuleByAllHeader()
{
	// TODO: �ڴ���������������
}

//αװģ�� �����ں˽��� ��Ҫע����� ģ��ĳ�������(����)ֻ��Ҫ�޸�ģ����PEB�е�·��
void CDlgModuleLooker::OnWeiZhuangModule()
{
	// TODO: �ڴ���������������
}
