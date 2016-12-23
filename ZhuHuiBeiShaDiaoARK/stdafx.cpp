
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ZhuHuiBeiShaDiaoARK.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

HANDLE	g_device = 0;
DWORD g_CurSelectPid = 0;
BOOL	g_bFirsetRun = FALSE;
CString g_CurSelectImage;
CString g_DriverName;
UCHAR	g_index;
CString g_ObjectName;
typedef long (*ZWUNMAPVIEWOFSECTION)(HANDLE,PVOID);
ZWUNMAPVIEWOFSECTION ZwUnmapViewOfSection;

typedef long (__stdcall *ZWOPENPROCESS)(
  OUT PHANDLE             ProcessHandle,
  IN ACCESS_MASK          AccessMask,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN PCLIENT_ID           ClientId );
ZWOPENPROCESS ZwOpenProcess;


typedef long (__stdcall *ZWOPENTHREAD)(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId );
ZWOPENTHREAD ZwOpenThread;

typedef long (__stdcall *ZWQUERYSYSTEMINFORMATION)(
    IN ULONG SystemInformationClass,
    IN OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    IN PULONG ReturnLength OPTIONAL );
ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;

typedef long (__stdcall *ZWQUERYINFORMATIONPROCESS)(
    IN HANDLE ProcessHandle,
    IN ULONG ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL);
ZWQUERYINFORMATIONPROCESS ZwQueryInformationProcess;

typedef long (__stdcall *ZWQUERYINFORMATIONTHREAD)(
	__in       HANDLE ThreadHandle,
	__in       ULONG ThreadInformationClass,
	__inout    PVOID ThreadInformation,
	__in       ULONG ThreadInformationLength,
	__out_opt  PULONG ReturnLength);
ZWQUERYINFORMATIONTHREAD ZwQueryInformationThread;

typedef long (__stdcall *RTLINITUNICODESTRING)(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString);
RTLINITUNICODESTRING RtlInitUnicodeString;

typedef long (*RTLADJUSTPRIVILEGE)(ULONG,ULONG,ULONG,PVOID);
RTLADJUSTPRIVILEGE RtlAdjustPrivilege;

typedef BOOL (__stdcall *ENUMPROCESSMODULES)(
  __in   HANDLE hProcess,
  __out  HMODULE *lphModule,
  __in   DWORD cb,
  __out  LPDWORD lpcbNeeded);
ENUMPROCESSMODULES EnumProcessModules;

typedef DWORD (__stdcall *GETMODULEFILENAMEEXA)(
  __in      HANDLE hProcess,
  __in_opt  HMODULE hModule,
  __out     LPCSTR lpFilename,
  __in      DWORD nSize);
GETMODULEFILENAMEEXA GetModuleFileNameExA;

typedef DWORD (__stdcall *GETMODULEFILENAMEEXW)(
  __in      HANDLE hProcess,
  __in_opt  HMODULE hModule,
  __out     LPWSTR lpFilename,
  __in      DWORD nSize);
GETMODULEFILENAMEEXW GetModuleFileNameExW;

typedef long (__stdcall *ZWTERMINATEPROCESS)(
  __in_opt  HANDLE ProcessHandle,
  __in      NTSTATUS ExitStatus);
ZWTERMINATEPROCESS ZwTerminateProcess;

typedef long (__stdcall *ZWTERMINATETHREAD)(
	__in_opt  HANDLE ProcessHandle,
	__in      NTSTATUS ExitStatus);
ZWTERMINATETHREAD ZwTerminateThread;

typedef long (__stdcall *ZWSUSPENDPROCESS)(
  __in_opt  HANDLE ProcessHandle);
ZWSUSPENDPROCESS ZwSuspendProcess;

typedef long (__stdcall *ZWRESUMEPROCESS)(
  __in_opt  HANDLE ProcessHandle);
ZWRESUMEPROCESS ZwResumeProcess;

typedef long (__stdcall *ZWSUSPENDTHREAD)(
	__in_opt	HANDLE ProcessHandle,
	__out_opt	PULONG PreviousSuspendCount);

typedef long (__stdcall *ZWRESUMETHREAD)(
	__in_opt	HANDLE ProcessHandle,
	__out_opt	PULONG SuspendCount);

ZWSUSPENDTHREAD ZwSuspendThread;

ZWRESUMETHREAD ZwResumeThread;

typedef long (*ZWSHUTDOWNSYSTEM)(LONG ShutdownAction);

typedef BOOL (WINAPI *ENUMPROCESSMODULESEX)(
	__in   HANDLE hProcess,
	__out  HMODULE *lphModule,
	__in   DWORD cb,
	__out  LPDWORD lpcbNeeded,
	__in   DWORD dwFilterFlag);

ZWSHUTDOWNSYSTEM ZwShutdownSystem;

ENUMPROCESSMODULESEX EnumProcessModulesEx;


void CopyToClipboard(HWND hWndOwner, LPCTSTR strText)
{
	if( !OpenClipboard(hWndOwner))
		return;
	EmptyClipboard();

	size_t cbStr = (_tcslen(strText) + 1) * sizeof(TCHAR);

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);
	memcpy_s(GlobalLock(hData), cbStr, strText, cbStr);
	GlobalUnlock(hData);

	// �ж���ʲô����
	UINT uiFormat = (sizeof(TCHAR) == sizeof(WCHAR)) ? CF_UNICODETEXT : CF_TEXT; 

	if (::SetClipboardData(uiFormat, hData) == NULL){
		DWORD dwErro = GetLastError(); 
		//AfxMessageBox();
	}
	CloseClipboard();

	GlobalFree(hData);
}

BOOL NtPathToDosPathW(WCHAR* FullNtPath, WCHAR* FullDosPath)
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

BOOL ARKDeviceIoControl(DWORD dwContryolCode,LPVOID inBuff, DWORD inBuffSize, LPVOID outBuff, DWORD outBuffSize)
{
	DWORD dwRet = 0;
	return DeviceIoControl(g_device,dwContryolCode, inBuff, inBuffSize, outBuff, outBuffSize,&dwRet,NULL);
}


BOOL GetFileInfo(WCHAR *pszFileFullPath, PWCHAR pszFileCorporation)
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


void LookFileAttrubute(WCHAR* FileFullName)
{
	HANDLE hFile = CreateFileW( FileFullName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL );
    if ( INVALID_HANDLE_VALUE == hFile )
    {
		AfxMessageBox(L"�ļ�������");
		return;
    }
	SHELLEXECUTEINFO   sei; 
	sei.hwnd   =   NULL; 
	sei.lpVerb   =  L"properties";
	sei.lpFile   =   FileFullName;
	sei.lpDirectory   =   NULL; 
	sei.lpParameters   =  NULL; 
	sei.nShow   =   SW_SHOWNORMAL; 
	sei.fMask   =   SEE_MASK_INVOKEIDLIST ; 
	sei.lpIDList   =   NULL; 
	sei.cbSize   =   sizeof(SHELLEXECUTEINFO); 
	ShellExecuteEx(&sei);
}

//���һ��������IsCheckAllΪFalse��Ч
ULONG CheckFileTrust(WCHAR* FileName,BOOL IsCheckAll,int subItem,BOOL IsShowNoSing)
{
	BOOL bRet = FALSE;
    WINTRUST_DATA wd = { 0 };
    WINTRUST_FILE_INFO wfi = { 0 };
    WINTRUST_CATALOG_INFO wci = { 0 };
    CATALOG_INFO ci = { 0 };
    HCATADMIN hCatAdmin = NULL;

    if ( !CryptCATAdminAcquireContext( &hCatAdmin, NULL, 0 ) )
    {
		//AfxMessageBox(L"CryptCATAdminAcquireContext");
        return FALSE;
    }
    HANDLE hFile = CreateFileW( FileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL );
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        CryptCATAdminReleaseContext( hCatAdmin, 0 );
		AfxMessageBox(L"�ļ�������");
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
				bRet = FILETRUST_SUCCESS;
			}
			if(hr==FILETRUST_EXPIRE){
				AfxMessageBox(L"�ļ�ǩ������!");
				bRet = FILETRUST_EXPIRE;
			}
			if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
				AfxMessageBox(L"�ļ�û��ǩ��");
				bRet = FILETRUST_NOSIGN;
			}
		}else
		{
			if(hr==S_OK){
				bRet = FILETRUST_SUCCESS;
			}
			if(hr==FILETRUST_EXPIRE){
				bRet = FILETRUST_EXPIRE;
			}
			if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
				bRet = FILETRUST_NOSIGN;
			}
		}
	}else
	{
		//���ﶯ̬����һ�� ��ʾǩ����� ���û�ˢ�µ�ʱ���ɾ�������
		if(hr==FILETRUST_EXPIRE)
		{
			//m_ListPro.SetItemText(subItem,8,L"����ǩ��");
			bRet = FILETRUST_EXPIRE;
		}
		if(hr==FILETRUST_NOSIGN){//�������ж��Ƿ���΢���ǩ�� ΢����ļ�ǩ������ǩ����Ҳ����û��ǩ��{
			//m_ListPro.SetItemText(subItem,8,L"δǩ��");
			bRet = FILETRUST_EXPIRE;
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


DWORD FileLen(char *filename)
{
	WIN32_FIND_DATAA fileInfo = { 0 };
	DWORD fileSize = 0;
	HANDLE hFind;
	hFind = FindFirstFileA(filename, &fileInfo);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		fileSize = fileInfo.nFileSizeLow;
		FindClose(hFind);
	}
	return fileSize;
}

CHAR *LoadDllContext(char *filename)
{
	DWORD dwReadWrite, LenOfFile = FileLen(filename);
	HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		PCHAR buffer = (PCHAR)malloc(LenOfFile);
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		ReadFile(hFile, buffer, LenOfFile, &dwReadWrite, 0);
		CloseHandle(hFile);
		return buffer;
	}
	return NULL;
}

ULONG64 GetWin32kImageBase()
{
	PIMAGE_NT_HEADERS64 pinths64;
	PIMAGE_DOS_HEADER pdih;
	char *NtosFileData = NULL;
	NtosFileData = LoadDllContext("c:\\win32k.dll");
	pdih = (PIMAGE_DOS_HEADER)NtosFileData;
	pinths64 = (PIMAGE_NT_HEADERS64)(NtosFileData + pdih->e_lfanew);
	return pinths64->OptionalHeader.ImageBase;
}