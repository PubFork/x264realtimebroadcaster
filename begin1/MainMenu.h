#pragma once
#include "afxcmn.h"
#include "Menu1.h"


#define WM_SHOWTASK2 WM_USER+2
// CMainMenu �Ի���

class CMainMenu : public CDialog
{
	DECLARE_DYNAMIC(CMainMenu)

public:
	CMainMenu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainMenu();

// �Ի�������
	enum { IDD = IDD_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabctrl;//tab�Ŀ��Ʊ���
	CMenu1 m_menu1;//�ӶԻ���ĳ�Ա����



	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CMainMenu* pDlgMM;
private:
	CFont cfont;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	NOTIFYICONDATA m_nid;//Ϊ��С����׼��
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
