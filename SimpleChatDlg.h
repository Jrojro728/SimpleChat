﻿
// SimpleChatDlg.h: 头文件
//

#pragma once


// CSimpleChatDlg 对话框
class CSimpleChatDlg : public CDialogEx
{
// 构造
public:
	CSimpleChatDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMPLECHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CEdit m_edit;
	CBitmapButton m_Btn1;
	afx_msg void OnBnClickedButton1();
	CEdit m_edit1;
	CEdit m_edit2;
	CButton m_Btn5;
	CButton m_Btn6;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	CEdit mEdit_4;
	CEdit mEdit_5; 
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton9();
	CEdit m_edit6;
};
