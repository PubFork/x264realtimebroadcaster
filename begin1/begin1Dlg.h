// begin1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#define WM_SHOWTASK2 WM_USER+2//为最小化定义消息

class Cbegin1Dlg : public CDialog
{
	// 构造
public:
	Cbegin1Dlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_BEGIN1_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit2();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	void ToTray();
	void DeleteTray();
public:	
	int abc;
	Cbegin1Dlg* pDlgbD;
	NOTIFYICONDATA nid;//为最小化做准备
	//NOTIFYICONDATA nid;  
	CString MMEnum;
	CString IPnum;
protected:
//	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_passkey;
	afx_msg void OnEnChangeIph();
};
