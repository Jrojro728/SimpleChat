
// SimpleChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SimpleChat.h"
#include "SimpleChatDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString AppAtTheDirectory();

CString FtpUserName("ftpuser");
CString FtpUserPassword("123456Jr");
CFtpConnection *Ftp;

CString new_Line("\n");
CString FileName(AppAtTheDirectory() + _T("\\聊天记录.txt"));
CString UserFileName(AppAtTheDirectory() + _T("\\用户信息.txt"));
CString NotConfigureFileName("聊天记录.txt");
CString NotConfigureUserFileName("用户信息.txt");

bool DevloperMode = false;

CString AppAtTheDirectory()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(250 + 1), 250);
	path.ReleaseBuffer();
	path.Trim(_T("SimpleChat.exe"));
	path.Insert(0, _T("C"));
	return path;
}

void inline GetFtpInternetSession()
{
	CString FtpServerUrl("192.168.1.106");
	CInternetSession * pInternetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	Ftp = pInternetSession->GetFtpConnection(FtpServerUrl, NULL, NULL, 21);
}

void FileWrite(CString & WriteFileName, CString & str)
{
	CStdioFile FileWrite;
	if (!FileWrite.Open(WriteFileName, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText))
	{
		AfxMessageBox(_T("打开文件失败!"));
		return;
	}
	DWORD dwFileLen = FileWrite.GetLength();
	//if (0 == dwFileLen)
	//{
	//	const unsigned char LeadBytes[] = { 0xEF, 0xBB, 0xBF };
	//	FileWrite.Write(LeadBytes, sizeof(LeadBytes));
	//}

	//开始转换utf8
	int nSrcLen = (int)wcslen(str);
	CStringA utf8String(str);

	int nBufLen = (nSrcLen + 1) * 6;
	LPSTR buffer = utf8String.GetBufferSetLength(nBufLen);
	int nLen = AtlUnicodeToUTF8(str, nSrcLen, buffer, nBufLen);

	buffer[nLen] = 0;
	utf8String.ReleaseBuffer();
	FileWrite.SeekToEnd(); //定位到最后

	FileWrite.Write(utf8String.GetBuffer(), nLen);//写入utf8字符串
	FileWrite.Write(new_Line.GetBuffer(), 1);
	FileWrite.Close();
}

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
	DDX_Control(pDX, IDC_BUTTON4, m_Btn1);
	DDX_Control(pDX, IDC_EDIT2, m_edit1);
	DDX_Control(pDX, IDC_EDIT3, m_edit2);
	DDX_Control(pDX, IDC_BUTTON5, m_Btn5);
	DDX_Control(pDX, IDC_BUTTON6, m_Btn6);
}

BEGIN_MESSAGE_MAP(CSimpleChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CSimpleChatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSimpleChatDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CSimpleChatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CSimpleChatDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSimpleChatDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSimpleChatDlg::OnBnClickedButton7)
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

	HINSTANCE hInstance0;
	HBITMAP hBitmap0;
	hInstance0 = ::AfxGetInstanceHandle();
	hBitmap0 = ::LoadBitmap(hInstance0, MAKEINTRESOURCE(IDB_BITMAP1));
	m_Btn1.SetBitmap(hBitmap0);

	HINSTANCE hInstance1;
	HBITMAP hBitmap1;
	hInstance1 = ::AfxGetInstanceHandle();
	hBitmap1 = ::LoadBitmap(hInstance1, MAKEINTRESOURCE(IDB_BITMAP2));
	m_Btn5.SetBitmap(hBitmap1);

	HINSTANCE hInstance2;
	HBITMAP hBitmap2;
	hInstance2 = ::AfxGetInstanceHandle();
	hBitmap2 = ::LoadBitmap(hInstance2, MAKEINTRESOURCE(IDB_BITMAP3));
	m_Btn6.SetBitmap(hBitmap2);

	GetFtpInternetSession();

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
	if (str == _T("123456789123456789"))//开发者模式
	{
		DevloperMode = true;
		AfxMessageBox(_T("test"));
		return;
	}
	FileWrite(FileName ,str);
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

void CSimpleChatDlg::OnBnClickedButton1()
{
	CString UserName;
	CString PassWord;

	m_edit1.GetWindowText(UserName);
	m_edit2.GetWindowText(PassWord);

	FileWrite(UserFileName, UserName);
	FileWrite(UserFileName, PassWord);
}

void CSimpleChatDlg::OnBnClickedButton5()
{
	/*AfxMessageBox(AppAtTheDirectory());*/
    Ftp->GetFile(NotConfigureFileName, FileName, false);
	Ftp->GetFile(NotConfigureUserFileName, UserFileName, false);
}


void CSimpleChatDlg::OnBnClickedButton6()
{
	/*AfxMessageBox(AppAtTheDirectory());*/
	Ftp->PutFile(FileName, NotConfigureFileName);
	Ftp->PutFile(UserFileName, NotConfigureUserFileName);
}


void CSimpleChatDlg::OnBnClickedButton7()
{
	CFile::Remove(FileName);
	CFile::Remove(UserFileName);
	OnBnClickedButton5();
}
