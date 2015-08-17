#include "dll\GlobalVariable.h"

#pragma once
// CMenu4 对话框
extern int PASCAL ConfigSGWIPandPort(char* SGW_IP_Delphi,
							  unsigned short SGW_PORT_Audio_Delphi,unsigned short SGW_PORT_Video_Delphi);

extern int PASCAL ConfigLOCALIPandPort(char* LOCAL_IP_Delphi,
								unsigned short LOCAL_RCV_PORT_Audio_Delphi,
								unsigned short LOCAL_SEND_PORT_Audio_Delphi,unsigned short LOCAL_RCV_PORT_Video_Delphi,
								unsigned short LOCAL_SEND_PORT_Video_Delphi);

extern int PASCAL ConfigDESTIPandPort(char* DEST_IP_Delphi,
									  unsigned short DEST_PORT_Audio_Delphi);

extern int PASCAL ConfigLOCALIPID( unsigned long LOCAL_IPID_Audio_Delphi, unsigned long LOCAL_IPID_Video_Delphi );
extern int PASCAL ConfigDESTIPID( unsigned long DEST_IPID_Audio_Delphi, unsigned long DEST_IPID_Video_Delphi );

extern int PASCAL OpenAudioSend();
extern int PASCAL CloseAudioSend();
extern int PASCAL OpenAudioSocketSend();
extern int PASCAL CloseAudioSocketSend();

extern int PASCAL OpenAudioReceive1();
extern int PASCAL OpenAudioReceive2();
extern int PASCAL OpenAudioSocketReceive();
extern int PASCAL CloseAudioSocketReceive();


extern int PASCAL OpenAudioRecord(char* WavFilePathandName_Delphi);
extern int PASCAL CloseAudioRecord();

extern int PASCAL OpenAudioReplay(char* AudioFilePathandName_Delphi);
extern int PASCAL CloseAudioReplay();

extern int PASCAL OpenVideoSocketSend();
extern int PASCAL  OpenVideoSocketReceive();
extern int PASCAL CloseVideoSocketSend();
extern int PASCAL  CloseVideoSocketReceive();
extern int PASCAL OpenVideoSend();
extern int PASCAL OpenVideoReceive();
extern int PASCAL CloseVideoSend();
extern int PASCAL CloseVideoReceive();

extern int PASCAL SystemInti();
extern int PASCAL CloseVideoSocketSend();
extern int PASCAL OpenVideoSocketReceive();
extern int PASCAL ConfigVideoDisplayerSet(DWORD HANDLE_DISPLAY_Delphi,
									 int coordinate_x_Delphi, int coordinate_y_Delphi,
									 int wsize_Delphi, int hsize_Delphi);

extern int PASCAL ConfigVideoDisplayerSetA(DWORD HANDLE_DISPLAY_Delphi,
									 int Acoordinate_x_Delphi, int Acoordinate_y_Delphi,
									 int Awsize_Delphi, int Ahsize_Delphi);



// CMenu1 对话框

class CMenu1 : public CDialog
{
	DECLARE_DYNAMIC(CMenu1)

public:
	CMenu1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMenu1();

	// 对话框数据
	enum { IDD = IDD_MENU1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	//	virtual void OnOK();
public:
	//int M1flagac=0;//是否接入通话的变量
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRadio22();
	afx_msg void OnBnClickedButton9();


	CString m_M1IP;
	char* m_M1IPc;	
	CString m_M1_C_A_S_Port;
	CString m_M1_C_A_R_Port;
	CString m_M1_C_V_S_Port;
	CString m_M1_C_V_R_Port;
	CString m_M1_S_A_S_Port;
	CString m_M1_S_A_R_Port;
	CString m_M1_S_V_S_Port;
	CString m_M1_S_V_R_Port;
	short m_M1_C_A_S_Ports;
	short m_M1_C_A_R_Ports;
	short m_M1_C_V_S_Ports;
	short m_M1_C_V_R_Ports;
	short m_M1_S_A_S_Ports;
	short m_M1_S_A_R_Ports;
	short m_M1_S_V_S_Ports;
	short m_M1_S_V_R_Ports;
	

	CString m_M1LState1;
	CString m_M1LState2;
	CString m_M1LIP2;
	CString m_M1LPort1;
	CString m_M1LPort2;
	CString m_M1LIP1;
	char* m_M1LIP2c;
	char* m_M1LIP1c;
	short m_M1LPort2s;
	short m_M1LPort1s;


	CMenu1* pDlgM1;
	//afx_msg void OnBnClickedButton17();
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedButton13();
	//afx_msg void OnBnClickedButton12();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	//afx_msg void OnBnClickedButton15();
	CString m_M1SIP;
	char *m_M1SIPc;
	short m_M1SAPort;
		short m_M1SVPort;
	afx_msg void OnBnClickedBAcall();
	//afx_msg void OnBnClickedM1lisclob();
	afx_msg void OnBnClickedBcallclose();
	int m_TraType;
	//afx_msg void OnBnClickedRadio14();
	//afx_msg void OnBnClickedRadio15();
	//afx_msg void OnBnClickedRadio21();
	//afx_msg void OnBnClickedAmrrate1();
	//afx_msg void OnBnClickedAmrrate2();
	//afx_msg void OnBnClickedAmrrate3();
	//afx_msg void OnBnClickedAmrrate4();
	//afx_msg void OnBnClickedAmrrate5();
	afx_msg void OnBnClickedBvcall();
	afx_msg void OnBnClickedBinit();
	//afx_msg void OnBnClickedVc1b();
	//afx_msg void OnBnClickedVc2b();
	//afx_msg void OnBnClickedRadio23();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton11();
	//afx_msg void OnBnClickedStaticL();

};
