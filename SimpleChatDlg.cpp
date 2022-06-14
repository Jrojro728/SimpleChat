﻿
// SimpleChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SimpleChat.h"
#include "SimpleChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString new_Line("\r\n");
CString FileName(_T("C:\\Users\\聊天记录.txt"));

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSimpleChatDlg 对话框



CSimpleChatDlg::CSimpleChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMPLECHAT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CSimpleChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CSimpleChatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSimpleChatDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CSimpleChatDlg 消息处理程序

BOOL CSimpleChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSimpleChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSimpleChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSimpleChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSimpleChatDlg::OnBnClickedButton3()
{
	CString str; //定义一个变量str
	m_edit.GetWindowText(str); //获取编辑框文本到str
	
	CStdioFile FileWrite;
	if (!FileWrite.Open(FileName, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText))
	{
		AfxMessageBox(_T("打开文件失败!"));
		return;
	}
	DWORD dwFileLen = FileWrite.GetLength();
	if (0 == dwFileLen)
	{
		const unsigned char LeadBytes[] = { 0xEF, 0xBB, 0xBF };
		FileWrite.Write(LeadBytes, sizeof(LeadBytes));
	}
	int nSrcLen = (int)wcslen(str);
	CStringA utf8String(str);

	int nBufLen = (nSrcLen + 1) * 6;
	LPSTR buffer = utf8String.GetBufferSetLength(nBufLen);
	int nLen = AtlUnicodeToUTF8(str, nSrcLen, buffer, nBufLen);

	buffer[nLen] = 0;
	utf8String.ReleaseBuffer();
	FileWrite.SeekToEnd();

	FileWrite.Write(utf8String.GetBuffer(), nLen);
	FileWrite.Write(new_Line.GetBuffer(), 2);
	FileWrite.Close();
}

void CSimpleChatDlg::OnBnClickedButton4()
{
	CStdioFile fileRead;
	if (!fileRead.Open(FileName, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(_T("文件无法打开!\n"));
		return;
	}
	
	DWORD len = fileRead.GetLength();
	char* fileData = new char[len + 1];
	fileData[len] = 0;
	fileRead.Read(fileData, len);

	CString str;
	str = CA2W(fileData, CP_UTF8);
	GetDlgItem(IDC_STATIC0)->SetWindowText(str);

	fileRead.Close();
}