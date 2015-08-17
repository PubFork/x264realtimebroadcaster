#pragma once
#include "afxcmn.h"
#include "Menu1.h"


#define WM_SHOWTASK2 WM_USER+2
// CMainMenu 对话框

class CMainMenu : public CDialog
{
	DECLARE_DYNAMIC(CMainMenu)

public:
	CMainMenu(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainMenu();

// 对话框数据
	enum { IDD = IDD_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabctrl;//tab的控制变量
	CMenu1 m_menu1;//子对话框的成员变量



	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CMainMenu* pDlgMM;
private:
	CFont cfont;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	NOTIFYICONDATA m_nid;//为最小化做准备
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void ToTray();
	void DeleteTray();
protected:
//	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int temp_tab_Mode;
	afx_msg void OnPaint();
};
