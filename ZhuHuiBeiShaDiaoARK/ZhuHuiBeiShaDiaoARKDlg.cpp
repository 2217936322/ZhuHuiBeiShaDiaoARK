
// ZhuHuiBeiShaDiaoARKDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZhuHuiBeiShaDiaoARK.h"
#include "ZhuHuiBeiShaDiaoARKDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnProcess32790();
	//afx_msg void UnloadDelFile();
	afx_msg void OnLookWindows();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_COMMAND(ID_PROCESS_32790, &CAboutDlg::OnProcess32790)
	//ON_COMMAND(ID_32800, &CAboutDlg::UnloadDelFile)
END_MESSAGE_MAP()


// CZhuHuiBeiShaDiaoARKDlg �Ի���



CZhuHuiBeiShaDiaoARKDlg::CZhuHuiBeiShaDiaoARKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZhuHuiBeiShaDiaoARKDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZhuHuiBeiShaDiaoARKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZhuHuiBeiShaDiaoARKDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CZhuHuiBeiShaDiaoARKDlg ��Ϣ�������

BOOL CZhuHuiBeiShaDiaoARKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//�鿴���������Ƿ����
	g_device = CreateFile(L"\\\\.\\MyDriver",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if(g_device == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"CreateFile is null ,May be driver load failed !",L"Error!");
		DestroyWindow();
	}

	this->m_sheet.AddPage(&m_DlgPro);
	this->m_sheet.AddPage(&m_DlgDriver);
	this->m_sheet.AddPage(&m_DlgKernel);
	this->m_sheet.AddPage(&m_DlgKerHook);
	this->m_sheet.AddPage(&m_DlgR3Hook);
	this->m_sheet.AddPage(&m_DlgNetWork);
	this->m_sheet.AddPage(&m_DlgFile);
	this->m_sheet.AddPage(&m_DlgSystemStart);
	this->m_sheet.AddPage(&m_DlgFunction);
	this->m_sheet.AddPage(&m_DlgRootFunc);
	this->m_sheet.AddPage(&m_DlgConfig);
	this->m_sheet.AddPage(&m_DlgAuthor);

	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	/*RECT rect;
	m_sheet.GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;*/

	m_sheet.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CZhuHuiBeiShaDiaoARKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZhuHuiBeiShaDiaoARKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZhuHuiBeiShaDiaoARKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






BOOL CZhuHuiBeiShaDiaoARKDlg::DestroyWindow()
{
	if(g_device != INVALID_HANDLE_VALUE)
		CloseHandle(g_device);
	return CDialogEx::DestroyWindow();
}
