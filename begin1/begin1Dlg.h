// begin1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#define WM_SHOWTASK2 WM_USER+2//Ϊ��С��������Ϣ

class Cbegin1Dlg : public CDialog
{
	// ����
public:
	Cbegin1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_BEGIN1_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	NOTIFYICONDATA nid;//Ϊ��С����׼��
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
