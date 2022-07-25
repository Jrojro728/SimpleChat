
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
DWORD GetHash(BYTE* pbData, DWORD dwDataLen, ALG_ID algId, LPTSTR pszHash);
DWORD UseHash(IN CString Str, OUT CString &HashStr);

CFtpConnection *Ftp;

CString new_Line("\n");
CString FileName(AppAtTheDirectory() + _T("\\聊天记录.txt"));
CString UserFileName(AppAtTheDirectory() + _T("\\用户信息.txt"));
CString NotConfigureFileName("聊天记录.txt");
CString NotConfigureUserFileName("用户信息.txt");
CString TempHashStr, TempHashStr2;

bool DevloperMode = false;

struct User
{
	CString UserName;
	CString PassWord;
	bool IsSetting = FALSE;
}user;

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
	CString FtpServerUrl("47.107.52.72");
	CInternetSession * pInternetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	Ftp = pInternetSession->GetFtpConnection(FtpServerUrl, NULL, NULL, 21);
}

void FileWrite(IN CString & WriteFileName, IN CString & str)
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

void FileWriteWithHash(IN CString& WriteFileName, IN CString& str)
{
	UseHash(str, TempHashStr);
	FileWrite(WriteFileName, TempHashStr);
}

void FileRead(IN CString & ReadFileName, OUT CString& str)
{
	CStdioFile fileRead;
	if (!fileRead.Open(ReadFileName, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(_T("文件无法打开!\n"));
		return;
	}

	DWORD len = fileRead.GetLength();
	char* fileData = new char[len + 1];
	fileData[len] = 0;
	fileRead.Read(fileData, len);

	str = CA2W(fileData, CP_UTF8);
	fileRead.Close();
}

HBITMAP MakeBitmapIcon(IN int BaseIDBBitmap)
{
	HINSTANCE hInstance;
	HBITMAP hBitmap;
	hInstance = ::AfxGetInstanceHandle();
	hBitmap = ::LoadBitmap(hInstance, MAKEINTRESOURCE(BaseIDBBitmap));
	return hBitmap;
}

DWORD GetHash(BYTE* pbData, DWORD dwDataLen, ALG_ID algId, LPTSTR pszHash)
{

	DWORD dwReturn = 0;
	HCRYPTPROV hProv;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return (dwReturn = GetLastError());

	HCRYPTHASH hHash;
	//Alg Id:CALG_MD5,CALG_SHA
	if (!CryptCreateHash(hProv, algId, 0, 0, &hHash))
	{
		dwReturn = GetLastError();
		CryptReleaseContext(hProv, 0);
		return dwReturn;
	}

	if (!CryptHashData(hHash, pbData, dwDataLen, 0))
	{
		dwReturn = GetLastError();
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return dwReturn;
	}

	DWORD dwSize;
	DWORD dwLen = sizeof(dwSize);
	CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)(&dwSize), &dwLen, 0);

	BYTE* pHash = new BYTE[dwSize];
	dwLen = dwSize;
	CryptGetHashParam(hHash, HP_HASHVAL, pHash, &dwLen, 0);

	lstrcpy(pszHash, _T(""));
	TCHAR szTemp[3];
	for (DWORD i = 0; i < dwLen; ++i)
	{
		//wsprintf(szTemp, _T("%X%X"), pHash[i] >> 4, pHash[i] & 0xf);
		wsprintf(szTemp, L"%02X", pHash[i]);
		lstrcat(pszHash, szTemp);	
	}
	delete[] pHash;

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return dwReturn;
}

DWORD UseHash(IN CString Str, OUT CString & HashStr)
{
	int StrSize{ ((int)wcslen(Str) * 6) + 1 };
	BYTE* StrByteBuffer = new BYTE[StrSize];
	char* StrCharBuffer = new char[StrSize];
	CString FinHashStr;
	
	WideCharToMultiByte(CP_ACP, 0, Str.GetBuffer(Str.GetLength()), -1, StrCharBuffer, NULL, NULL, NULL);
	memcpy(StrByteBuffer, StrCharBuffer, StrSize);
	DWORD status = GetHash(StrByteBuffer, StrSize, CALG_MD5, FinHashStr.GetBufferSetLength(32 + 1));

	HashStr = FinHashStr;
	return status;
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
}

BEGIN_MESSAGE_MAP(CSimpleChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CSimpleChatDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSimpleChatDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CSimpleChatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CSimpleChatDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CSimpleChatDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON2, &CSimpleChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CSimpleChatDlg::OnBnClickedButton8)
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

	
	m_Btn1.SetBitmap(MakeBitmapIcon(IDB_BITMAP1));
	m_Btn5.SetBitmap(MakeBitmapIcon(IDB_BITMAP2));

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



void CSimpleChatDlg::OnBnClickedButton3() //写入
{
	CString str; //定义一个变量str
	m_edit.GetWindowText(str); //获取编辑框文本到str
	if (str == _T("123456789123456789"))//开发者模式
	{
		DevloperMode = true;
		return;
	}
	if(user.IsSetting == TRUE)
		str.Insert(0, user.UserName + L": ");
	FileWrite(FileName ,str);

	Ftp->PutFile(FileName, NotConfigureFileName);
	Ftp->PutFile(UserFileName, NotConfigureUserFileName);
}

void CSimpleChatDlg::OnBnClickedButton4() //读取
{
	CString str;
	FileRead(FileName , str);
	GetDlgItem(IDC_STATIC0)->SetWindowText(str);
}

void CSimpleChatDlg::OnBnClickedButton1() //写入
{
	CString UserName;
	CString PassWord;

	m_edit1.GetWindowText(UserName);
	m_edit2.GetWindowText(PassWord);

	FileWriteWithHash(UserFileName, UserName);
	FileWriteWithHash(UserFileName, PassWord);
}

void CSimpleChatDlg::OnBnClickedButton5() //从服务器下载文件
{
	/*AfxMessageBox(AppAtTheDirectory());*/
    Ftp->GetFile(NotConfigureFileName, FileName, false);
	Ftp->GetFile(NotConfigureUserFileName, UserFileName, false);
}

void CSimpleChatDlg::OnBnClickedButton7() //重置
{
	CFile::Remove(FileName);
	CFile::Remove(UserFileName);
	OnBnClickedButton5();
}


void CSimpleChatDlg::OnBnClickedButton2() //读取
{
	CString ReadStr;
	CString TestUserName;
	CString TestPassWord;

	m_edit1.GetWindowText(TestUserName);
	m_edit2.GetWindowText(TestPassWord);

	UseHash(TestUserName, TempHashStr);
	UseHash(TestUserName, TempHashStr2);

	FileRead(UserFileName, ReadStr);
	if (ReadStr.Find(TempHashStr) >= 0 && ReadStr.Find(TempHashStr2) >= 0)
	{
		user.UserName = TestUserName;
		user.PassWord = TestPassWord;
		user.IsSetting = TRUE;
	}
	else
	{
		AfxMessageBox(_T("登录错误:用户名或密码错误!"));
		return;
	}
}

void CSimpleChatDlg::OnBnClickedButton8()
{
	CDialogEx::OnOK();
}
