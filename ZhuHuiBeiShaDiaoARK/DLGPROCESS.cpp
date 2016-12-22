// DLGPROCESS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "DLGPROCESS.h"
#include "afxdialogex.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "DlgModuleLooker.h"
#include "ZhuHuiBeiShaDiaoARKDlg.h"


#include "DlgHandleLook.h"
#include "DlgThreadLooker.h"

#pragma comment(lib,"version.lib")

#define MY_SUCCESS 0
#define MY_OPENPROCESSFaild 1
#define MY_DELFILEFAILD 2
#define MY_KILLPROCESSFAILD 3




IMPLEMENT_DYNAMIC(CDLGPROCESS, CPropertyPage)

CDLGPROCESS::CDLGPROCESS()
	: CPropertyPage(CDLGPROCESS::IDD)
	 //m_CurSelectPid(0)
	, nItemSelct(0)
	, m_AllProcessNum(0)
{

}

CDLGPROCESS::~CDLGPROCESS()
{
}

void CDLGPROCESS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListPro);
}


BEGIN_MESSAGE_MAP(CDLGPROCESS, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDLGPROCESS::OnNMRClickList1)
	ON_COMMAND(ID_PROCESS_32771, &CDLGPROCESS::OnProcess32771)
	ON_COMMAND(ID_PROCESS_32796, &CDLGPROCESS::OnlyShow64bitProc)
	ON_COMMAND(ID_PROCESS_32797, &CDLGPROCESS::OnlyShow32bitProc)
	ON_COMMAND(ID_PROCESS_32793, &CDLGPROCESS::OpenPathName)
	ON_COMMAND(ID_PROCESS_32790, &CDLGPROCESS::CopyImageName)
	ON_COMMAND(ID_PROCESS_32772, &CDLGPROCESS::OnLookModule)
	ON_COMMAND(ID_PROCESS_32776, &CDLGPROCESS::OnZwKillProcess)
	//ON_COMMAND(ID_PROCESS_32813, &CDLGPROCESS::OnProcess32813)
	ON_COMMAND(ID_PROCESS_32813, &CDLGPROCESS::OnZwTerByKernel)
	ON_COMMAND(ID_PROCESS_32782, &CDLGPROCESS::OnKillProcessAndDelByR3)
	ON_COMMAND(ID_PROCESS_32814, &CDLGPROCESS::OnKillPrcessAndDelByR0)
	ON_COMMAND(ID_Menu32783, &CDLGPROCESS::OnRootDelFile)
	ON_COMMAND(ID_PROCESS_32786, &CDLGPROCESS::OnSuspendProcess)
	ON_COMMAND(ID_PROCESS_32787, &CDLGPROCESS::OnResumeProcess)
	ON_COMMAND(ID_PROCESS_32784, &CDLGPROCESS::OnCheckFileTrust)
	ON_COMMAND(ID_PROCESS_32785, &CDLGPROCESS::OnCheckFileAll)
	ON_COMMAND(ID_PROCESS_32773, &CDLGPROCESS::OnLookThread)
	ON_COMMAND(ID_PROCESS_32774, &CDLGPROCESS::OnLookHandle)
	ON_COMMAND(ID_Menu, &CDLGPROCESS::OnTimer)
	ON_COMMAND(ID_32779, &CDLGPROCESS::OnLookMemory)
	ON_COMMAND(ID_32780, &CDLGPROCESS::OnLookWindows)
	ON_COMMAND(ID_PROCESS_32815, &CDLGPROCESS::OnLookHanldeByR0)
	ON_COMMAND(ID_PROCESS_32791, &CDLGPROCESS::OnCopyPathName)
	ON_COMMAND(ID_PROCESS_32794, &CDLGPROCESS::OnLookFileAttribute)
	ON_COMMAND(ID_PROCESS_32788, &CDLGPROCESS::OnProcessWeiZhuang)
	ON_COMMAND(ID_PROCESS_32789, &CDLGPROCESS::OnProcessHide)
	ON_COMMAND(ID_Menu32781, &CDLGPROCESS::OnForceKillProcess)
	ON_COMMAND(ID_PROCESS_32795, &CDLGPROCESS::OnDownShowModule)
	ON_COMMAND(ID_PROCESS_32816, &CDLGPROCESS::OnShowNoSingPro)
END_MESSAGE_MAP()


// CDLGPROCESS ��Ϣ�������

char *GetProcessPath(HANDLE hProcess)
{
	HMODULE hMods[2048];
    DWORD cbNeeded;
	char *retstr=(char *)malloc(MAX_PATH);
	memset(retstr,0x0,MAX_PATH);
    if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
		if(GetModuleFileNameExA(hProcess, hMods[0], retstr,MAX_PATH))
		{
			//GetShortPathNameA(retstr,retstr,MAX_PATH); //���ܶ��ļ���
			return retstr;
		}
		else
			return NULL;
    }
	return NULL;
}



DWORD GetParentProcessId(HANDLE hProc)
{
	ULONG retlng;
	PROCESS_BASIC_INFORMATION pbi;
	memset(&pbi,0x0,sizeof(pbi));
	ZwQueryInformationProcess(hProc,0,&pbi,sizeof(pbi),&retlng);
	return (DWORD)pbi.InheritedFromUniqueProcessId;
}

void GetProcessPathName(DWORD processID,WCHAR* Outbuff)
{
	 HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	 //TCHAR* procName = new TCHAR[MAX_PATH];

	 GetModuleFileNameExW(hProcess, NULL, Outbuff, MAX_PATH);
	 CloseHandle(hProcess);
	 //return procName;
}

BOOL CDLGPROCESS::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	ULONG dwRetVal=0;
	InitAPI();
	//��ʾȨ��
	RtlAdjustPrivilege(20,1,0,&dwRetVal);//debug
	RtlAdjustPrivilege(19,1,0,&dwRetVal);
	m_Ico.Create(16, 16, ILC_COLOR32, 10, 10);
	//m_Ico.Add(NULL);
	m_ListPro.SetImageList(&m_Ico, LVSIL_SMALL);
	//LVS_EX_AUTOSIZECOLUMNS �Զ����� ������ֹ�����
	m_ListPro.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES);
	m_ListPro.InsertColumn(0,L"��������",0,150);
	m_ListPro.InsertColumn(1,L"����ID",LVCFMT_CENTER,50);
	m_ListPro.InsertColumn(2,L"��ID",LVCFMT_CENTER,50);
	m_ListPro.InsertColumn(3,L"PE-Bit",LVCFMT_CENTER,60);
	m_ListPro.InsertColumn(4,L"����·��",0,200);
	m_ListPro.InsertColumn(5,L"���̶���",LVCFMT_CENTER,130);
	m_ListPro.InsertColumn(6,L"����״̬",LVCFMT_CENTER,70);
	m_ListPro.InsertColumn(7,L"�ļ�����",0,160);
	EnumProByOpenPro(0);
	
	m_IsCheckFileState = FALSE;
	return TRUE;
}

void CharToWChar(wchar_t* Desbuff,char* src)
{
	size_t len = strlen(src) + 1;
	size_t converted = 0;
	mbstowcs_s(&converted, Desbuff, len, src, _TRUNCATE);
}

void CharToWChar2(wchar_t* Desbuff,char* src)
{ 
	size_t nc = strlen(src);  
	size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)src, (int)nc, NULL, 0);  
	MultiByteToWideChar(CP_ACP, 0, (const char *)src, (int)nc, Desbuff, (int)n);  
}

BOOL CDLGPROCESS::GetFileInfo(WCHAR *pszFileFullPath, PWCHAR pszFileCorporation)
{
	DWORD	dwXHandle=0;
	DWORD	dwXInfoSize;
	LPBYTE	lpXBuffer;
	UINT	uDataSize = 0;
	LPVOID	lpXData=NULL;
	int		iStringLength=0;
	WCHAR	textBuffer[256]={'\0'};

	//////////////////////////////////////////////////////////////////////////
	dwXInfoSize=GetFileVersionInfoSizeW(pszFileFullPath, &dwXHandle);
	if (dwXInfoSize != 0)
	{
		lpXBuffer=new BYTE[dwXInfoSize];				//����汾��Ϣ��С���ڴ�
		memset(lpXBuffer, 0, dwXInfoSize*sizeof(BYTE));	//���㣬���ڱ���汾��Ϣ
		//��ȡ�ļ��汾��Ϣ
		if (GetFileVersionInfoW(pszFileFullPath, dwXHandle, dwXInfoSize, lpXBuffer))
		{
			//�ļ��������Ϣ
			struct LANGANDCODEPAGE 
			{
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;

			if( !VerQueryValueW(lpXBuffer, L"\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &uDataSize) )
				free(lpXBuffer);

			//��˾
			wsprintf(textBuffer,L"\\StringFileInfo\\%04x%04x\\CompanyName",
							lpTranslate[0].wLanguage,
							lpTranslate[0].wCodePage);

			//�ļ��Ҽ���"����"->"�汾"->"��˾"
			if( VerQueryValueW(lpXBuffer, textBuffer, &lpXData, &uDataSize) )
				StrCpyW( (WCHAR *)pszFileCorporation, LPTSTR(lpXData) );	//����File Corporation
		}
		free(lpXBuffer);
		lpXBuffer=NULL;

		return TRUE;
	}
	else	//��ȡ�ļ��汾��Ϣ��Сʧ�ܣ������ļ��Ƿ����
	{
		//"ֻ��"��ʽ���ļ�
		//hFile=fopen(pszFileFullPath, "r");
		//if ( !hFile )
		//{
		//	//�ų�"Idle"��"System"2���������·��
		//	if (_stricmp(pszFileFullPath, "Idle") == 0)
		//	{
		//		ZeroMemory( (char *)pszFileCorporation, strlen(pszFileCorporation) );
		//		return FALSE;
		//	}
		//	if (_stricmp(pszFileFullPath, "System") == 0)
		//	{
		//		ZeroMemory( (char *)pszFileCorporation, strlen(pszFileCorporation) );
		//		return FALSE;
		//	}

		//	strcpy( (CHAR *)pszFileCorporation, "�ļ�������" );
		//	return FALSE;
		//}
		//else
		//{
		//	ZeroMemory( (char *)pszFileCorporation, strlen(pszFileCorporation) );
		//	fclose(hFile);
		//}
		StrCpyW(pszFileCorporation,L"��Ϣ������");
	}

	return FALSE;
}

void CDLGPROCESS::EnumProByOpenPro(int sign)
{
	CString str,tempstr,numstr;
	HANDLE	hNtProcess=0,hTemp=0;
	DWORD	i=0;
	WCHAR	*Wtemp = (WCHAR*)malloc(512);
	WCHAR	*pzFile = (WCHAR*)malloc(512);

	memset(Wtemp,'\0',512);
	memset(pzFile,'\0',512);
	int  Icoindex = 0;
	HICON	hicon;
	HINSTANCE hinst=AfxGetInstanceHandle();

	m_AllProcessNum = 0;
	m_Ico.Remove(-1);

	for(i=0;i<20000;i+=4)
	{
		hNtProcess  = NtdllOpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,0,i); 
		if (hNtProcess !=0)
		{
			char *ProcPath=GetProcessPath(hNtProcess);
			if(sign==0) //show all
			{	
				if (ProcPath!=NULL)
				{
					if(GetParentProcessId(hNtProcess)!=4)
					{
						int nIntemNum = m_ListPro.GetItemCount();
						//ͼ���ʼ��
						
						GetProcessPathName(i,Wtemp);
						str.Format(L"%s",Wtemp);
						hicon = 0;
						hicon = ExtractIcon(hinst,Wtemp,0);
						if(hicon==NULL)
						{
							hicon = LoadIcon(hinst,MAKEINTRESOURCE(IDI_ICON1));
						}
						Icoindex = m_Ico.Add(hicon);
						if(IsWow64(hNtProcess))
							 str = str + L" *32";//QQ.exe *32;
						memset(Wtemp,'\0',512);
						//��·���õ�������
				
						tempstr = str.Mid(str.ReverseFind('\\')+1);
						m_ListPro.InsertItem(nIntemNum,tempstr,Icoindex);


						str.Format(L"%d",i);//PID
						m_ListPro.SetItemText(nIntemNum,1,str);

						str.Format(L"%d",GetParentProcessId(hNtProcess));//PPID
						m_ListPro.SetItemText(nIntemNum,2,str);

						if(!IsWow64(hNtProcess))//PE+  -��64bit
						{
							m_ListPro.SetItemText(nIntemNum,3,L"64bit");
						}else
						{
							m_ListPro.SetItemText(nIntemNum,3,L"32bit");
						}

						CharToWChar2(Wtemp,ProcPath);//����·��
						str.Format(L"%s",Wtemp);
						m_ListPro.SetItemText(nIntemNum,4,str);
						memset(Wtemp,'\0',512);

						str = GetEprocess(i);
						if(str == L"NULL")
							m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");//EPROCESS
						else
							m_ListPro.SetItemText(nIntemNum,5,str);//EPROCESS

						hTemp = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_VM_WRITE,0,i);
						if(hTemp!=0)
						{
							m_ListPro.SetItemText(nIntemNum,6,L"-");
							hTemp = 0;
							CloseHandle(hTemp);
						}else
						{
							m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
						}
						
						GetProcessPathName(i,Wtemp);
						GetFileInfo(Wtemp,pzFile);
						str.Format(L"%s",pzFile);
						m_ListPro.SetItemText(nIntemNum,7,str);
						/*if(str==L"Microsoft Corporation")
							m_ListPro.SetTextColor(RGB(58,58,255));*/
						memset(pzFile,'\0',512);
						memset(Wtemp,'\0',512);
						m_AllProcessNum++;
					}else
					{
						str.Format(L"%d",i);
						int nIntemNum = m_ListPro.GetItemCount();
						m_ListPro.InsertItem(nIntemNum,L"smss.exe");
						m_ListPro.SetItemText(nIntemNum,1,str);
						m_ListPro.SetItemText(nIntemNum,2,L"4");
						m_ListPro.SetItemText(nIntemNum,3,L"64bit");
						m_ListPro.SetItemText(nIntemNum,4,L"C:\\Windows\\System32\\smss.exe");
						str = GetEprocess(i);
						if(str == L"NULL")
							m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");
						else
							m_ListPro.SetItemText(nIntemNum,5,str);
						m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
						m_ListPro.SetItemText(nIntemNum,7,L"Microsoft Corporation");
						m_AllProcessNum++;
					}
				}
					
			}
			else if(sign==1) //show no sign
			{
				if (ProcPath!=NULL && GetParentProcessId(hNtProcess)!=4)
				{
					memset(Wtemp,'\0',512);
					CharToWChar2(Wtemp,ProcPath);//����·��

					if(!CheckFileTrust(Wtemp,FALSE,0,TRUE))//�����1000�Ǳ���� ��Ϊ������ܵ�һ���ж�����
					{
						int nIntemNum = m_ListPro.GetItemCount();
						//ͼ���ʼ��
						
						GetProcessPathName(i,Wtemp);
						str.Format(L"%s",Wtemp);
						hicon = 0;
						hicon = ExtractIcon(hinst,Wtemp,0);
						if(hicon==NULL)
						{
							hicon = LoadIcon(hinst,MAKEINTRESOURCE(IDI_ICON1));
						}
						Icoindex = m_Ico.Add(hicon);
						if(IsWow64(hNtProcess))
							 str = str + L" *32";//QQ.exe *32;
						memset(Wtemp,'\0',512);
						//��·���õ�������
				
						tempstr = str.Mid(str.ReverseFind('\\')+1);
						m_ListPro.InsertItem(nIntemNum,tempstr,Icoindex);


						str.Format(L"%d",i);//PID
						m_ListPro.SetItemText(nIntemNum,1,str);

						str.Format(L"%d",GetParentProcessId(hNtProcess));//PPID
						m_ListPro.SetItemText(nIntemNum,2,str);

						if(!IsWow64(hNtProcess))//PE+  -��64bit
						{
							m_ListPro.SetItemText(nIntemNum,3,L"64bit");
						}else
						{
							m_ListPro.SetItemText(nIntemNum,3,L"32bit");
						}

						CharToWChar2(Wtemp,ProcPath);//����·��
						str.Format(L"%s",Wtemp);
						m_ListPro.SetItemText(nIntemNum,4,str);
						memset(Wtemp,'\0',512);

						str = GetEprocess(i);
						if(str == L"NULL")
							m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");
						else
							m_ListPro.SetItemText(nIntemNum,5,str);

						hTemp = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_VM_WRITE,0,i);
						if(hTemp!=0)
						{
							m_ListPro.SetItemText(nIntemNum,6,L"-");
							hTemp = 0;
							CloseHandle(hTemp);
						}else
						{
							m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
						}
						
						GetProcessPathName(i,Wtemp);
						GetFileInfo(Wtemp,pzFile);
						str.Format(L"%s",pzFile);
						m_ListPro.SetItemText(nIntemNum,7,str);
						/*if(str==L"Microsoft Corporation")
							m_ListPro.SetTextColor(RGB(58,58,255));*/
						memset(pzFile,'\0',512);
						memset(Wtemp,'\0',512);
						m_AllProcessNum++;//û��ǩ���Ľ��̸���
					}
				}
			}
			else if(sign==2) //only show 32b process
			{
				if (ProcPath!=NULL)
				{
					if(IsWow64(hNtProcess)==1)
					{
						int nIntemNum = m_ListPro.GetItemCount();
						//ͼ���ʼ��
						
						GetProcessPathName(i,Wtemp);
						str.Format(L"%s",Wtemp);
						hicon = 0;
						hicon = ExtractIcon(hinst,Wtemp,0);
						if(hicon==NULL)
						{
							hicon = LoadIcon(hinst,MAKEINTRESOURCE(IDI_ICON1));
						}
						Icoindex = m_Ico.Add(hicon);
						if(IsWow64(hNtProcess))
							 str = str + L" *32";//QQ.exe *32;
						memset(Wtemp,'\0',512);
						//��·���õ�������
				
						tempstr = str.Mid(str.ReverseFind('\\')+1);
						m_ListPro.InsertItem(nIntemNum,tempstr,Icoindex);


						str.Format(L"%d",i);//PID
						m_ListPro.SetItemText(nIntemNum,1,str);

						str.Format(L"%d",GetParentProcessId(hNtProcess));//PPID
						m_ListPro.SetItemText(nIntemNum,2,str);

						if(!IsWow64(hNtProcess))//PE+  -��64bit
						{
							m_ListPro.SetItemText(nIntemNum,3,L"64bit");
						}else
						{
							m_ListPro.SetItemText(nIntemNum,3,L"32bit");
						}

						CharToWChar2(Wtemp,ProcPath);//����·��
						str.Format(L"%s",Wtemp);
						m_ListPro.SetItemText(nIntemNum,4,str);
						memset(Wtemp,'\0',512);

						str = GetEprocess(i);
						if(str == L"NULL")
							m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");
						else
							m_ListPro.SetItemText(nIntemNum,5,str);

						hTemp = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_VM_WRITE,0,i);
						if(hTemp!=0)
						{
							m_ListPro.SetItemText(nIntemNum,6,L"-");
							hTemp = 0;
							CloseHandle(hTemp);
						}else
						{
							m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
						}
						
						GetProcessPathName(i,Wtemp);
						GetFileInfo(Wtemp,pzFile);
						str.Format(L"%s",pzFile);
						m_ListPro.SetItemText(nIntemNum,7,str);
						/*if(str==L"Microsoft Corporation")
							m_ListPro.SetTextColor(RGB(58,58,255));*/
						memset(pzFile,'\0',512);
						memset(Wtemp,'\0',512);
						m_AllProcessNum++;
					}
						
				}
			}			
			else if(sign==3) //only show 64b process
			{
				if (ProcPath!=NULL)
				{
					if(IsWow64(hNtProcess)==0)
					{
						if(GetParentProcessId(hNtProcess)==4)
						{
							str.Format(L"%d",i);
							int nIntemNum = m_ListPro.GetItemCount();
							m_ListPro.InsertItem(nIntemNum,L"smss.exe");
							m_ListPro.SetItemText(nIntemNum,1,str);
							m_ListPro.SetItemText(nIntemNum,2,L"4");
							m_ListPro.SetItemText(nIntemNum,3,L"64bit");
							m_ListPro.SetItemText(nIntemNum,4,L"C:\\Windows\\System32\\smss.exe");

							str = GetEprocess(i);
							if(str == L"NULL")
								m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");
							else
								m_ListPro.SetItemText(nIntemNum,5,str);

							m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
							m_ListPro.SetItemText(nIntemNum,7,L"Microsoft Corporation");
							m_AllProcessNum++;
						}else
						{
							int nIntemNum = m_ListPro.GetItemCount();
							//ͼ���ʼ��
						
							GetProcessPathName(i,Wtemp);
							str.Format(L"%s",Wtemp);
							hicon = 0;
							hicon = ExtractIcon(hinst,Wtemp,0);
							if(hicon==NULL)
							{
								hicon = LoadIcon(hinst,MAKEINTRESOURCE(IDI_ICON1));
							}
							Icoindex = m_Ico.Add(hicon);
							if(IsWow64(hNtProcess))
								 str = str + L" *32";//QQ.exe *32;
							memset(Wtemp,'\0',512);
							//��·���õ�������
				
							tempstr = str.Mid(str.ReverseFind('\\')+1);
							m_ListPro.InsertItem(nIntemNum,tempstr,Icoindex);


							str.Format(L"%d",i);//PID
							m_ListPro.SetItemText(nIntemNum,1,str);

							str.Format(L"%d",GetParentProcessId(hNtProcess));//PPID
							m_ListPro.SetItemText(nIntemNum,2,str);

							if(!IsWow64(hNtProcess))//PE+  -��64bit
							{
								m_ListPro.SetItemText(nIntemNum,3,L"64bit");
							}else
							{
								m_ListPro.SetItemText(nIntemNum,3,L"32bit");
							}

							CharToWChar2(Wtemp,ProcPath);//����·��
							str.Format(L"%s",Wtemp);
							m_ListPro.SetItemText(nIntemNum,4,str);
							memset(Wtemp,'\0',512);

							str = GetEprocess(i);
							if(str == L"NULL")
								m_ListPro.SetItemText(nIntemNum,5,L"0xFFFFFFFFFFFFFFFF");
							else
								m_ListPro.SetItemText(nIntemNum,5,str);

							hTemp = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_VM_WRITE,0,i);
							if(hTemp!=0)
							{
								m_ListPro.SetItemText(nIntemNum,6,L"-");
								hTemp = 0;
								CloseHandle(hTemp);
							}else
							{
								m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
							}
						
							GetProcessPathName(i,Wtemp);
							GetFileInfo(Wtemp,pzFile);
							str.Format(L"%s",pzFile);
							m_ListPro.SetItemText(nIntemNum,7,str);
							/*if(str==L"Microsoft Corporation")
								m_ListPro.SetTextColor(RGB(58,58,255));*/
							memset(pzFile,'\0',512);
							memset(Wtemp,'\0',512);
							m_AllProcessNum++;
						}
						
					}
						
				}
			}
			CloseHandle(hNtProcess);
		}
	}
	free(Wtemp);
	free(pzFile);
	if(sign!=2 && sign!=3){
	int nIntemNum = m_ListPro.GetItemCount();
	m_ListPro.InsertItem(nIntemNum,L"System Idle Process");
	m_ListPro.SetItemText(nIntemNum,1,L"0");
	m_ListPro.SetItemText(nIntemNum,2,L"-");
	m_ListPro.SetItemText(nIntemNum,3,L"-");
	m_ListPro.SetItemText(nIntemNum,4,L"Idl");
	m_ListPro.SetItemText(nIntemNum,5,L"");
	m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
	m_ListPro.SetItemText(nIntemNum,7,L"Microsoft Corporation");

	//int nIntemNum = m_ListPro.GetItemCount();
	m_ListPro.InsertItem(nIntemNum,L"System");
	m_ListPro.SetItemText(nIntemNum,1,L"4");
	m_ListPro.SetItemText(nIntemNum,2,L"-");
	m_ListPro.SetItemText(nIntemNum,3,L"-");
	m_ListPro.SetItemText(nIntemNum,4,L"System");
	m_ListPro.SetItemText(nIntemNum,5,L"");
	m_ListPro.SetItemText(nIntemNum,6,L"�ܾ�");
	m_ListPro.SetItemText(nIntemNum,7,L"Microsoft Corporation");
	numstr.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d ������",m_AllProcessNum+2);
	AfxGetMainWnd()->SetWindowTextW(numstr);
	}
	if(sign==2)
	{
		numstr.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d �� 32 λ����",m_AllProcessNum);
		AfxGetMainWnd()->SetWindowTextW(numstr);
	}
	if(sign==3)
	{
		numstr.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d �� 64 λ����",m_AllProcessNum);
		AfxGetMainWnd()->SetWindowTextW(numstr);
	}
	if(sign==1)
	{
		numstr.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d �� δǩ������",m_AllProcessNum);
		AfxGetMainWnd()->SetWindowTextW(numstr);
	}
	
}


void CDLGPROCESS::InitAPI(void)
{
	LoadLibraryW(L"user32.dll");
	ZwUnmapViewOfSection=(ZWUNMAPVIEWOFSECTION)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwUnmapViewOfSection");
	ZwSuspendProcess=(ZWSUSPENDPROCESS)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwSuspendProcess");
	ZwResumeProcess=(ZWRESUMEPROCESS)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwResumeProcess");
	ZwTerminateProcess=(ZWTERMINATEPROCESS)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwTerminateProcess");
	ZwTerminateThread=(ZWTERMINATETHREAD)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwTerminateThread");
	ZwSuspendThread=(ZWSUSPENDTHREAD)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwSuspendThread");
	ZwResumeThread=(ZWRESUMETHREAD)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwResumeThread");
	ZwQuerySystemInformation=(ZWQUERYSYSTEMINFORMATION)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwQuerySystemInformation");
	ZwOpenProcess=(ZWOPENPROCESS)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwOpenProcess");
	ZwOpenThread=(ZWOPENTHREAD)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwOpenThread");
	ZwQueryInformationProcess=(ZWQUERYINFORMATIONPROCESS)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwQueryInformationProcess");
	ZwQueryInformationThread=(ZWQUERYINFORMATIONTHREAD)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwQueryInformationThread");
	ZwShutdownSystem=(ZWSHUTDOWNSYSTEM)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"ZwShutdownSystem");
	RtlAdjustPrivilege=(RTLADJUSTPRIVILEGE)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"RtlAdjustPrivilege");
	RtlInitUnicodeString=(RTLINITUNICODESTRING)GetProcAddress(LoadLibraryW(L"ntdll.dll"),"RtlInitUnicodeString");
	GetModuleFileNameExA=(GETMODULEFILENAMEEXA)GetProcAddress(LoadLibraryW(L"psapi.dll"),"GetModuleFileNameExA");
	GetModuleFileNameExW=(GETMODULEFILENAMEEXW)GetProcAddress(LoadLibraryW(L"psapi.dll"),"GetModuleFileNameExW");
	EnumProcessModules=(ENUMPROCESSMODULES)GetProcAddress(LoadLibraryW(L"psapi.dll"),"EnumProcessModules");
	EnumProcessModulesEx=(ENUMPROCESSMODULESEX)GetProcAddress(LoadLibraryW(L"psapi.dll"),"EnumProcessModulesEx");
}


BOOL CDLGPROCESS::IsWow64(HANDLE hProc)
{
	//hProcess=NtdllOpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,0,PID); 
	//IsWow64(hProcess) �����HANDLE����PID ��openprocess�õ��ľ��
    BOOL bIsWow64 = FALSE;
	IsWow64Process(hProc, &bIsWow64);
    return bIsWow64;
}

HANDLE CDLGPROCESS::NtdllOpenProcess(ACCESS_MASK AccessMask, BOOL bInheritHandle, DWORD dwProcessId)
{
	//hProcess=NtdllOpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,0,PID); 
	OBJECT_ATTRIBUTES oa;
	CLIENT_ID cid;
	memset(&oa,0x0,sizeof(oa));
	memset(&cid,0x0,sizeof(cid));
	HANDLE hProc=0;
	oa.Length=sizeof(oa);
	cid.UniqueProcess=(HANDLE)dwProcessId;
	ZwOpenProcess(&hProc,AccessMask,&oa,&cid);
	return hProc;
}

//�����Ҽ��˵�
void CDLGPROCESS::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		*pResult = 0;
	CMenu menu;
	BOOL IsSuspendProcess = FALSE;
	menu.LoadMenuW(IDR_MENU1);
	CMenu *pM = menu.GetSubMenu(0);
	
	CPoint pt;
	GetCursorPos(&pt);

	nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelct!=-1)
		IsSuspendProcess = m_ListPro.IsExistForArry(nItemSelct);
	if(!IsSuspendProcess)
	{
		menu.EnableMenuItem(ID_PROCESS_32787,MF_GRAYED);//���� �ָ����̹���
		menu.EnableMenuItem(ID_PROCESS_32786,MF_ENABLED);//���� ��ͣ���̹���
	}
	else
	{
		menu.EnableMenuItem(ID_PROCESS_32786,MF_GRAYED);//���� ��ͣ���̹���
		menu.EnableMenuItem(ID_PROCESS_32787,MF_ENABLED);//���� �ָ����̹���
	}
	pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	*pResult = 0;
}


void CDLGPROCESS::OnProcess32771()//ˢ��
{
	if(m_IsCheckFileState)
	{
		m_ListPro.DeleteColumn(8);
		m_IsCheckFileState = FALSE;
		m_ListPro.Invalidate(NULL);
	}
	m_ListPro.DeleteAllItems();
	EnumProByOpenPro(0);
}


void CDLGPROCESS::OnlyShow64bitProc()//show 64bit proc
{
	m_ListPro.DeleteAllItems();
	EnumProByOpenPro(3);

}


void CDLGPROCESS::OnlyShow32bitProc()//show 32bit proc
{
	m_ListPro.DeleteAllItems();
	EnumProByOpenPro(2);
}


void CDLGPROCESS::OpenPathName()//��Ŀ¼
{
	int curitem = 0;
	CString str = 0;
	curitem = m_ListPro.GetNextItem(-1,LVIS_SELECTED); 
	if(curitem!=-1)
		str = m_ListPro.GetItemText(curitem,4);
	str = L"/select," + str;
	ShellExecute(0,L"open",L"Explorer.exe",str,NULL,SW_NORMAL);
}

//��������·��
void CDLGPROCESS::OnCopyPathName()
{
	CString str;
	str = GetItemInfo(4);
	CopyToClipboard(m_hWnd,str);
}

//����������
void CDLGPROCESS::CopyImageName()
{
	CString str = 0;
	str = GetItemInfo(0);
	CopyToClipboard(m_hWnd,str);
}

//�鿴ģ��
void CDLGPROCESS::OnLookModule()
{
	int Curitem = 0;
	CString str;
	Curitem = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
	if(Curitem!=-1)
	{
		str = m_ListPro.GetItemText(Curitem,1);
		g_CurSelectImage = m_ListPro.GetItemText(Curitem,0);
	}else
	{
		AfxMessageBox(L"��û��ѡ�����");
		return;
	}
	
	g_CurSelectPid  = _ttoi(str);
	CDlgModuleLooker DlgModule;
	DlgModule.DoModal();
}

//Ӧ�ò��������
void CDLGPROCESS::OnZwKillProcess()
{
	if(MyKillProcessAndDelFile(_wtoi(GetItemInfo(1)),FALSE)!=MY_SUCCESS)
		AfxMessageBox(L"����ʧ�� ��ʹ��ǿ�ƽ�������!");
}

//��ȡ�б�ؼ���Ϣ
CString CDLGPROCESS::GetItemInfo(int subItem)
{
	nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelct!=-1)
		return m_ListPro.GetItemText(nItemSelct,subItem);
	return NULL;
}

//�ں�������� ���������ڻ�ȡ�������̾��ʱʹ�� ��Ȼ���ʱ��R3��ö�ٽ��̷���Ҳ�ͷ��� ��Ҫ���ں�ö�� �������ö�ٽ��̵ķ�������R3��ɵ� ��ô���ѡ���ֹ
void CDLGPROCESS::OnZwTerByKernel()
{
	//Ψһ��ͬ���Ǵ򿪽��̵ķ�ʽ��ͬ
	//ʹ�����ַ���Ҳ��˵���Ǹ�����ʹ����ĳ�ֱ��� ʹ�ù��߻�ȡ�������
}

//������������ɾ��
void CDLGPROCESS::OnKillProcessAndDelByR3()
{
	int Ret = MyKillProcessAndDelFile(_wtoi(GetItemInfo(1)),TRUE);
	if(Ret!=MY_SUCCESS)
	{
		if(Ret==MY_DELFILEFAILD)
			AfxMessageBox(L"ɾ���ļ�ʧ�ܣ�������\"������ɾ��\" ���� \"ǿ��ɾ��\"");
		if(Ret==MY_KILLPROCESSFAILD)
			AfxMessageBox(L"����ʧ�� ��ʹ��ǿ�ƽ�������!");
		if(Ret==MY_OPENPROCESSFaild)
			AfxMessageBox(L"�򿪽���ʧ�ܣ���ʹ�ù���Ա���л�ʹ��ǿ�ƹ���!");
	}
}

//����ɾ���ļ�
void CDLGPROCESS::OnRootDelFile()
{
	MoveFileExW(GetItemInfo(4),NULL,MOVEFILE_DELAY_UNTIL_REBOOT);
	AfxMessageBox(L"��ӳɹ�!������ɾ���ļ�!ȡ����ֱ�Ӹ���һ��!");
}


//����0 ˵�������ɹ� 1�򿪽���ʧ�� 2ɾ���ļ�ʧ�� 3��������ʧ��
int CDLGPROCESS::MyKillProcessAndDelFile(DWORD pid, bool IsDelFile)
{
	HANDLE hProcess = 0;
	CString str;
	long ZwRet = 0;
	hProcess = NtdllOpenProcess(PROCESS_ALL_ACCESS,0,pid);
	if(hProcess>0){
		ZwRet = ZwTerminateProcess(hProcess,0);
		CloseHandle(hProcess);

		if(ZwRet!=0){
			return MY_KILLPROCESSFAILD;
		}
		
		str.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d ������",m_AllProcessNum+1);//pid 0 4 �������һ��
		AfxGetMainWnd()->SetWindowTextW(str);
		if(IsDelFile){
			str = GetItemInfo(4);
			nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
			m_ListPro.DeleteItem(nItemSelct);
			if(DeleteFile(str))
			{
				return MY_SUCCESS;
			}else
			{
				return MY_DELFILEFAILD;
			}	
		}else
		{
			nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
			m_ListPro.DeleteItem(nItemSelct);
			return MY_SUCCESS;
		}
			
	}
	return MY_OPENPROCESSFaild;
}

//��ͣ����
void CDLGPROCESS::OnSuspendProcess()
{
	HANDLE hProcess = 0;
	nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelct!=-1){
		m_ListPro.SetItemTextColor(nItemSelct,RGB(186,12,4),FALSE);
		hProcess = NtdllOpenProcess(PROCESS_ALL_ACCESS,0,_wtoi(GetItemInfo(1)));
		if(hProcess>0)
		{
			ZwSuspendProcess(hProcess);
			CloseHandle(hProcess);
		}
	}
}

//�ָ�����
void CDLGPROCESS::OnResumeProcess()
{
	HANDLE hProcess = 0;
	g_CurSelectPid = _wtoi(GetItemInfo(1));
	nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
	if(nItemSelct!=-1){
		m_ListPro.SetItemTextColor(nItemSelct,RGB(255,255,255),TRUE);
		hProcess = NtdllOpenProcess(PROCESS_ALL_ACCESS,0,g_CurSelectPid);
		if(hProcess>0)
		{
			ZwResumeProcess(hProcess);
			CloseHandle(hProcess);
		}
	}
}

//����һ���ļ���ǩ��
void CDLGPROCESS::OnCheckFileTrust()
{
	CString FileName;
	FileName = GetItemInfo(4);
	CheckFileTrust(FileName.GetBuffer(),FALSE,0,FALSE);
}

//����ȫ���ļ���ǩ��
void CDLGPROCESS::OnCheckFileAll()
{
	int ItemNum = m_ListPro.GetItemCount();
	m_ListPro.InsertColumn(8,L"ǩ��������",LVCFMT_CENTER,100);
	CString tempFilenName;
	m_IsCheckFileState = TRUE;
	for (int i = 0; i < ItemNum; i++)
	{
		tempFilenName = m_ListPro.GetItemText(i,4);
		AfxGetMainWnd()->SetWindowTextW(tempFilenName);
		CheckFileTrust(tempFilenName.GetBuffer(),TRUE,i,FALSE);
	}
	m_ListPro.Invalidate(NULL);
	AfxGetMainWnd()->SetWindowTextW(L"У�����!�������ʾ�����һ��!���û����ʾ!���������!");
}


//���һ��������IsCheckAllΪFalse��Ч
BOOL CDLGPROCESS::CheckFileTrust(WCHAR* FileName,BOOL IsCheckAll,int subItem,BOOL IsShowNoSing)
{
	BOOL bRet = FALSE;
    WINTRUST_DATA wd = { 0 };
    WINTRUST_FILE_INFO wfi = { 0 };
    WINTRUST_CATALOG_INFO wci = { 0 };
    CATALOG_INFO ci = { 0 };
    HCATADMIN hCatAdmin = NULL;

    if ( !CryptCATAdminAcquireContext( &hCatAdmin, NULL, 0 ) )
    {
        return FALSE;
    }
    HANDLE hFile = CreateFileW( FileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL );
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        CryptCATAdminReleaseContext( hCatAdmin, 0 );
        return FALSE;
    }
    DWORD dwCnt = 100;
    BYTE byHash[100];
    CryptCATAdminCalcHashFromFileHandle( hFile, &dwCnt, byHash, 0 );
    CloseHandle( hFile );
    LPWSTR pszMemberTag = new WCHAR[dwCnt * 2 + 1];
    HCATINFO hCatInfo = CryptCATAdminEnumCatalogFromHash( hCatAdmin,byHash, dwCnt, 0, NULL );

        if ( NULL == hCatInfo )
    {
        wfi.cbStruct = sizeof( WINTRUST_FILE_INFO );
		wfi.pcwszFilePath =  FileName;
        wfi.hFile = NULL;
        wfi.pgKnownSubject = NULL;
        wd.cbStruct = sizeof( WINTRUST_DATA );
        wd.dwUnionChoice = WTD_CHOICE_FILE;
        wd.pFile = &wfi;
        wd.dwUIChoice = WTD_UI_NONE;
        wd.fdwRevocationChecks = WTD_REVOKE_NONE;
        wd.dwStateAction = WTD_STATEACTION_IGNORE;
        wd.dwProvFlags = WTD_SAFER_FLAG;
        wd.hWVTStateData = NULL;
        wd.pwszURLReference = NULL;
    }
    else
    {
        CryptCATCatalogInfoFromContext( hCatInfo, &ci, 0 );
        wci.cbStruct = sizeof( WINTRUST_CATALOG_INFO );
        wci.pcwszCatalogFilePath = ci.wszCatalogFile;
		wci.pcwszMemberFilePath = FileName;
        wci.pcwszMemberTag = pszMemberTag;
        wd.cbStruct = sizeof( WINTRUST_DATA );
        wd.dwUnionChoice = WTD_CHOICE_CATALOG;
        wd.pCatalog = &wci;
        wd.dwUIChoice = WTD_UI_NONE;
        wd.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
        wd.dwProvFlags = 0;
        wd.hWVTStateData = NULL;
        wd.pwszURLReference = NULL;
    }
    GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    HRESULT hr = WinVerifyTrust( NULL, &action, &wd );
	
	if(!IsCheckAll)
	{
		if(!IsShowNoSing)
		{
			if(hr==S_OK){
				AfxMessageBox(L"�ļ���ǩ��!");
				bRet = TRUE;
			}
			if(hr==FILETRUST_EXPIRE){
				AfxMessageBox(L"�ļ�ǩ������!");
				bRet = FALSE;
			}
			if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
				AfxMessageBox(L"�ļ�û��ǩ��");
				bRet = FALSE;
			}
		}else
		{
			if(hr==S_OK){
				bRet = TRUE;
			}
			if(hr==FILETRUST_EXPIRE){
				bRet = FALSE;
			}
			if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
				bRet = FALSE;
			}
		}
	}else
	{
		//���ﶯ̬����һ�� ��ʾǩ����� ���û�ˢ�µ�ʱ���ɾ�������
		if(hr==FILETRUST_EXPIRE)
		{
			m_ListPro.SetItemText(subItem,8,L"����ǩ��");
			bRet = FALSE;
		}
		if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
			m_ListPro.SetItemText(subItem,8,L"δǩ��");
			bRet = FALSE;
		}
	}
    if ( NULL != hCatInfo )
    {
        CryptCATAdminReleaseCatalogContext( hCatAdmin, hCatInfo, 0 );
    }
    CryptCATAdminReleaseContext(hCatAdmin, 0 );
    delete[] pszMemberTag;
    return bRet;
}

//�鿴�߳�
void CDLGPROCESS::OnLookThread()
{
	CString str;
	str = GetItemInfo(1);
	g_CurSelectPid = _ttoi(str);
	g_CurSelectImage = GetItemInfo(0);
	CDlgThreadLooker dlg;
	dlg.DoModal();
}

//�鿴���r3
void CDLGPROCESS::OnLookHandle()
{
	CString str;
	str = GetItemInfo(1);
	g_CurSelectPid  = _ttoi(str);
	g_CurSelectImage = GetItemInfo(0);
	CDlgHandleLook dlg;
	dlg.DoModal();
}

//�鿴��ʱ��
void CDLGPROCESS::OnTimer()
{
	
}

//�鿴�ڴ�ֲ�
void CDLGPROCESS::OnLookMemory()
{
	
}

//�鿴����
void CDLGPROCESS::OnLookWindows()
{
	
}

//�鿴���R0
void CDLGPROCESS::OnLookHanldeByR0()
{
	// TODO: �ڴ���������������
}


//�鿴�ļ�����
void CDLGPROCESS::OnLookFileAttribute()
{
	CString str;
	str = GetItemInfo(4);
	LookFileAttrubute(str.GetBuffer());
}

//����αװ
void CDLGPROCESS::OnProcessWeiZhuang()
{
	// TODO: �ڴ���������������
}

//��������
void CDLGPROCESS::OnProcessHide()
{
	// TODO: �ڴ���������������
}

//ǿ�ƽ������� ʹ��PspTerminthread
void CDLGPROCESS::OnForceKillProcess()
{
	CString str;
	ULONGLONG outBuff = 0;
	DWORD dwRet = 0;

	str = GetItemInfo(1);

	if(str)
		g_CurSelectPid  = _ttoi(str);

	if(DeviceIoControl(g_device,
						IOCTL_KILLPROCESS,
						&g_CurSelectPid,
						sizeof(DWORD),
						&outBuff,
						sizeof(ULONGLONG),
						&dwRet,NULL))
	{
		nItemSelct = m_ListPro.GetNextItem(-1,LVIS_SELECTED);
		if(nItemSelct != -1)
			m_ListPro.DeleteItem(nItemSelct);
		str.Format(L"��ᱻɱ��:[���̹�����]��ǰϵͳ���� %d ������",m_AllProcessNum+1);//pid 0 4 �������һ��
		AfxGetMainWnd()->SetWindowTextW(str);
	}
		

}

//ǿ�ƽ������̲�ǿ��ɾ���ļ�
void CDLGPROCESS::OnKillPrcessAndDelByR0()
{
	
}

//���·���ʾģ��
void CDLGPROCESS::OnDownShowModule()
{

}

//��ȡEprocess ʧ�ܷ����ַ���0
CString CDLGPROCESS::GetEprocess(DWORD pid)
{
	CString str;
	BOOL Io_Success = FALSE;
	ULONGLONG Buff = 0;
	DWORD dwRet = 0;

	Io_Success = DeviceIoControl(g_device,IOCTL_GETEPROCESS, &pid, sizeof(DWORD), &Buff, sizeof(ULONGLONG),&dwRet,NULL);

	if(Io_Success)
		str.Format(L"0x%p",Buff);
	else
		str = L"NULL";

	return str;
}

//����ʾ��ǩ������
void CDLGPROCESS::OnShowNoSingPro()
{
	m_ListPro.DeleteAllItems();
	//���ｨ�鿪�߳� ̫����
	EnumProByOpenPro(1);
}
