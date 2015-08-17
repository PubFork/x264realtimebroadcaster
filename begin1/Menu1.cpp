// Menu1.cpp : 实现文件
//
#include "stdafx.h"
#include "begin1.h"
#include "Menu1.h"
#include "MenuGlobal.h"
#include "vfw.h"
// CMenu1 对话框

IMPLEMENT_DYNAMIC(CMenu1, CDialog)

CMenu1::CMenu1(CWnd* pParent /*=NULL*/)
: CDialog(CMenu1::IDD, pParent)
, m_M1LState1(_T(""))
, m_M1LState2(_T(""))
, m_M1LIP2(_T(""))
, m_M1LPort1(_T(""))
,m_M1LPort2(_T(""))
, m_M1LIP1(_T(""))
, m_M1SIP(_T(""))
, m_M1SAPort(0)
, m_M1SVPort(0)
, m_TraType(0)
{

}

CMenu1::~CMenu1()
{
}

void CMenu1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_M1STATE1, m_M1LState1);
	//DDX_Text(pDX, IDC_M1STATE2, m_M1LState2);
	//DDX_Text(pDX, IDC_M1LIP2, m_M1LIP2);
	//DDX_Text(pDX, IDC_M1LPORT1, m_M1LPort1);
	//DDX_Text(pDX, IDC_M1LPORT2, m_M1LPort2);
	//DDX_Text(pDX, IDC_M1LIP1, m_M1LIP1);
	DDX_Text(pDX, IDC_IP12, m_M1SIP);
	DDX_Text(pDX, IDC_PORT13, m_M1SAPort);
		DDX_Text(pDX, IDC_PORT14, m_M1SVPort);
	//	DDX_Control(pDX, IDC_RADIO14, m_TraType);
}


BEGIN_MESSAGE_MAP(CMenu1, CDialog)
	//ON_BN_CLICKED(IDC_RADIO22, &CMenu1::OnBnClickedRadio22)
	//ON_BN_CLICKED(IDC_AC1B, &CMenu1::OnBnClickedButton9)
	//ON_BN_CLICKED(IDC_M1LISB , &CMenu1::OnBnClickedButton17)
	//ON_BN_CLICKED(IDC_ACCLOSE1B   , &CMenu1::OnBnClickedButton13)
	//ON_BN_CLICKED(IDC_ACCLOSE2B   , &CMenu1::OnBnClickedButton12)
	ON_WM_CREATE()
	//ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_AC2B , &CMenu1::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BACALL, &CMenu1::OnBnClickedBAcall)
	//ON_BN_CLICKED(IDC_M1LISCLOB, &CMenu1::OnBnClickedM1lisclob)
	ON_BN_CLICKED(IDC_BCALLCLOSE, &CMenu1::OnBnClickedBcallclose)
	//ON_BN_CLICKED(IDC_RADIO14, &CMenu1::OnBnClickedRadio14)
	//ON_BN_CLICKED(IDC_RADIO15, &CMenu1::OnBnClickedRadio15)
	//ON_BN_CLICKED(IDC_RADIO21, &CMenu1::OnBnClickedRadio21)
	//ON_BN_CLICKED(IDC_AMRRATE1, &CMenu1::OnBnClickedAmrrate1)
	//ON_BN_CLICKED(IDC_AMRRATE2, &CMenu1::OnBnClickedAmrrate2)
	//ON_BN_CLICKED(IDC_AMRRATE3, &CMenu1::OnBnClickedAmrrate3)
	//ON_BN_CLICKED(IDC_AMRRATE4, &CMenu1::OnBnClickedAmrrate4)
	//ON_BN_CLICKED(IDC_AMRRATE5, &CMenu1::OnBnClickedAmrrate5)
//	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BVCALL, &CMenu1::OnBnClickedBvcall)
	ON_BN_CLICKED(IDC_BINIT, &CMenu1::OnBnClickedBinit)
	//ON_BN_CLICKED(IDC_VC1B, &CMenu1::OnBnClickedVc1b)
	//ON_BN_CLICKED(IDC_VC2B, &CMenu1::OnBnClickedVc2b)
	//ON_BN_CLICKED(IDC_RADIO23, &CMenu1::OnBnClickedRadio23)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, &CMenu1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON11, &CMenu1::OnBnClickedButton11)
	//ON_BN_CLICKED(IDC_STATIC_L, &CMenu1::OnBnClickedStaticL)

END_MESSAGE_MAP()


// CMenu1 消息处理程序

//void CMenu1::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	//CDialog::OnOK();
//}

BOOL CMenu1::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
		switch(pMsg->wParam)
	{case VK_RETURN://回车
	return TRUE;
		case VK_ESCAPE://esc
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//void CMenu1::OnBnClickedRadio22()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}



//void CMenu1::OnBnClickedButton17()
//{
//	GetDlgItem(IDC_BACALL)->EnableWindow(false);
//	//GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(false);
//	//GetDlgItem(IDC_AC1B)->EnableWindow(true);
//	//GetDlgItem(IDC_AC2B)->EnableWindow(true);
//	//GetDlgItem(IDC_VC1B)->EnableWindow(true);
//	//GetDlgItem(IDC_VC2B)->EnableWindow(true);
//	//GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
//	//GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
//	//GetDlgItem(IDC_M1LISB)->EnableWindow(false);
//	GetDlgItem(IDC_BINIT)->EnableWindow(false);
//	//GetDlgItem(IDC_M1LISCLOB)->EnableWindow(true);
//
//	//GetDlgItem(IDC_AMRRATE1)->EnableWindow(false);
//	//GetDlgItem(IDC_AMRRATE2)->EnableWindow(false);
//	//GetDlgItem(IDC_AMRRATE3)->EnableWindow(false);
//	//GetDlgItem(IDC_AMRRATE4)->EnableWindow(false);
//	//GetDlgItem(IDC_AMRRATE5)->EnableWindow(false);
//
//
//	Is_Call=0;
////********************LDS配置视频播放
//	//LDS 配置播放窗口
//	CWnd *pWndStatev1 = NULL;  
//	pWndStatev1 = GetDlgItem(IDC_PIC1);
//	CRect rectL;
//	pWndStatev1->GetWindowRect(&rectL);//获取控件相对于屏幕的位置	
//	//ScreenToClient(rectL);//转化为对话框上的相对位置
//	  ConfigVideoDisplayerSetA(0, 0,0 , 176, 144);
//	ConfigVideoDisplayerSet(0, rectL.left,rectL.top,rectL.right-rectL.left, rectL.bottom-rectL.top);
//
////LDS 配置视频播放
//	ConfigLOCALIPID( 10, 30 );
//	ConfigDESTIPID( 10, 30 );
//	SystemInti();
//	OpenVideoSend();
//	OpenVideoReceive();
//
//
////********************LDS配置音频播放
//	OpenAudioSocketReceive();
//	OpenAudioSend();
//	OpenAudioReceive1();
//
//}

BOOL CMenu1::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_BVCALL)->EnableWindow(false);
	GetDlgItem(IDC_BACALL)->EnableWindow(false);
	GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(false);
	//GetDlgItem(IDC_AC1B)->EnableWindow(false);
	//GetDlgItem(IDC_AC2B)->EnableWindow(false);
	//GetDlgItem(IDC_VC1B)->EnableWindow(false);
	//GetDlgItem(IDC_VC2B)->EnableWindow(false);
	//GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
	//GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
	//GetDlgItem(IDC_M1LISCLOB)->EnableWindow(false);
	//GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(false);

	pDlgM1=this;
	m_M1_C_A_S_Port=Local_C_A_S_Port;
	m_M1_C_A_R_Port=Local_C_A_R_Port;
	m_M1_C_V_S_Port=Local_C_V_S_Port;
	m_M1_C_V_R_Port=Local_C_V_R_Port;
	m_M1_S_A_S_Port=Local_S_A_S_Port;
	m_M1_S_A_R_Port=Local_S_A_R_Port;
	m_M1_S_V_S_Port=Local_S_V_S_Port;
	m_M1_S_V_R_Port=Local_S_V_R_Port;
	m_M1_C_A_S_Ports=(short)atoi( m_M1_C_A_S_Port);
	m_M1_C_A_R_Ports=(short)atoi(  m_M1_C_A_R_Port);
	m_M1_C_V_S_Ports=(short)atoi(  m_M1_C_V_S_Port);
	m_M1_C_V_R_Ports=(short)atoi(  m_M1_C_V_R_Port);
	m_M1_S_A_S_Ports=(short)atoi(m_M1_S_A_S_Port);
	m_M1_S_A_R_Ports=(short)atoi(  m_M1_S_A_R_Port);
	m_M1_S_V_S_Ports=(short)atoi( m_M1_S_V_S_Port);
	m_M1_S_V_R_Ports=(short)atoi(  m_M1_S_V_R_Port);

	 m_M1SIP="172.16.0.117";
	m_M1SAPort=3400;
	m_M1SVPort=3402;
 	UpdateData(false);
	m_M1IP=LocalIP;//将全局变量传递给Menu1
	LPTSTR m_M1IPc=(LPTSTR)(LPCTSTR)m_M1IP;
	ConfigLOCALIPandPort(m_M1IPc,m_M1_S_A_R_Ports,m_M1_S_A_S_Ports,m_M1_S_V_R_Ports,m_M1_S_V_S_Ports);//192.168.3.169

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CMenu1::OnBnClickedButton13()
//{
//	GetDlgItem(IDC_AC1B)->EnableWindow(true);
//	GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
//	M4IP1.Empty();//LDS添加for showing UDP IP
//	M4PORT1.Empty();
//	CloseAudioSocketSend();
//	CloseAudioSocketReceive();
//	CloseVideoSocketSend();
//	CloseVideoSocketReceive();
//}

//void CMenu1::OnBnClickedButton12()
//{
//	GetDlgItem(IDC_AC2B)->EnableWindow(true);
//	GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
//	M4IP2.Empty();//LDS添加for showing UDP IP
//	M4PORT2.Empty();
//	CloseAudioSocketSend();
//	CloseAudioSocketReceive();
//	CloseVideoSocketSend();
//	CloseVideoSocketReceive();
//}

int CMenu1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetTimer(TIMER3,3000,0);

	return 0;
}

//void CMenu1::OnTimer(UINT_PTR nIDEvent)
//{
//	switch(nIDEvent)//&&pDlgM1
//	{
//	case TIMER3:
//		{
//			if(Is_Call==0)
//			{m_M1LIP1=M4IP1;
//		m_M1LPort1=M4PORT1;
//		m_M1LIP2=M4IP2;
//		m_M1LPort2=M4PORT2;
//
//		if(M4IP1.IsEmpty()!=0)
//			m_M1LState1="空闲";
//		else
//			m_M1LState1="在线";
//		if(M4IP2.IsEmpty()!=0)
//			m_M1LState2="空闲";
//		else
//			m_M1LState2="在线";
//
//		//GetDlgItem(IDC_M1LIP1)->SetWindowText(m_M1LIP1);
//		//GetDlgItem(IDC_M1LIP2)->SetWindowText(m_M1LIP2);
//		//GetDlgItem(IDC_M1LPORT2)->SetWindowText(m_M1LPort2);
//		//GetDlgItem(IDC_M1LPORT1)->SetWindowText(m_M1LPort1);
//		//GetDlgItem(IDC_M1LPORT2)->SetWindowText(m_M1LPort2);
//		//GetDlgItem(IDC_M1LPORT1)->SetWindowText(m_M1LPort1);
//		//GetDlgItem(IDC_M1STATE1)->SetWindowText(m_M1LState1);
//		//GetDlgItem(IDC_M1STATE2)->SetWindowText(m_M1LState2);
//			}
//		break;}
//	default:
//		break;
//		CFrameWnd(nIDEvent);
//	}
//
//
//	CDialog::OnTimer(nIDEvent);
//}

//void CMenu1::OnBnClickedButton15()
//{
//	GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(true);
//	GetDlgItem(IDC_AC2B)->EnableWindow(false);
//	GetDlgItem(IDC_AC1B)->EnableWindow(false);
//
//	Acess_Num=2;
//	LPTSTR m_M1LIP2c=(LPTSTR)(LPCTSTR)M4IP2;
//	m_M1LPort2s=(short)atoi(M4PORT2)-1;
//	ConfigSGWIPandPort( m_M1LIP2c,m_M1LPort2s,m_M1LPort2s+2);
//	OpenVideoSocketSend();
//}
//void CMenu1::OnBnClickedButton9()
//{
//	GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(true);
//	GetDlgItem(IDC_AC1B)->EnableWindow(false);
//	GetDlgItem(IDC_AC2B)->EnableWindow(false);
//
//	Acess_Num=1;
//	LPTSTR m_M1LIP1c=(LPTSTR)(LPCTSTR)M4IP1;
//	m_M1LPort1s=(short)atoi(M4PORT1)-1;
//	ConfigSGWIPandPort( m_M1LIP1c,m_M1LPort1s,m_M1LPort1s+2);
//	OpenVideoSocketSend();
//}

void CMenu1::OnBnClickedBAcall()
{ 		
	GetDlgItem(IDC_BACALL)->EnableWindow(false);
	GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(true);
	//GetDlgItem(IDC_AC1B)->EnableWindow(false);
	//GetDlgItem(IDC_AC2B)->EnableWindow(false);
	//GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
	//GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
	//GetDlgItem(IDC_M1LISB)->EnableWindow(false);
	//GetDlgItem(IDC_M1LISCLOB)->EnableWindow(false);

	//GetDlgItem(IDC_AMRRATE1)->EnableWindow(false);
	//GetDlgItem(IDC_AMRRATE2)->EnableWindow(false);
	//GetDlgItem(IDC_AMRRATE3)->EnableWindow(false);
	//GetDlgItem(IDC_AMRRATE4)->EnableWindow(false);
	//GetDlgItem(IDC_AMRRATE5)->EnableWindow(false);

	OpenAudioSocketSend();
	OpenAudioSocketReceive();

}

//void CMenu1::OnBnClickedM1lisclob()
//{
//	GetDlgItem(IDC_BACALL)->EnableWindow(true);
//	//GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(true);
//	GetDlgItem(IDC_AC1B)->EnableWindow(false);
//	GetDlgItem(IDC_AC2B)->EnableWindow(false);
//	GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
//	GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
//	GetDlgItem(IDC_M1LISB)->EnableWindow(true);
//	GetDlgItem(IDC_M1LISCLOB)->EnableWindow(false);
//	GetDlgItem(IDC_M1STATE1)->EnableWindow(false);
//	GetDlgItem(IDC_M1STATE2)->EnableWindow(false);
//	GetDlgItem(IDC_AC1B)->EnableWindow(false);
//	GetDlgItem(IDC_AC2B)->EnableWindow(false);
//	GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(false);
//	GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(false);
//	GetDlgItem(IDC_VC1B)->EnableWindow(false);
//	GetDlgItem(IDC_VC2B)->EnableWindow(false);
//
//		GetDlgItem(IDC_AMRRATE1)->EnableWindow(true);
//	GetDlgItem(IDC_AMRRATE2)->EnableWindow(true);
//	GetDlgItem(IDC_AMRRATE3)->EnableWindow(true);
//	GetDlgItem(IDC_AMRRATE4)->EnableWindow(true);
//	GetDlgItem(IDC_AMRRATE5)->EnableWindow(true);
//
//}

void CMenu1::OnBnClickedBcallclose()
{	
	GetDlgItem(IDC_BACALL)->EnableWindow(true);
	GetDlgItem(IDC_BVCALL)->EnableWindow(true);
	//GetDlgItem(IDC_AC1B)->EnableWindow(true);
	//GetDlgItem(IDC_AC2B)->EnableWindow(true);
	//GetDlgItem(IDC_ACCLOSE1B)->EnableWindow(true);
	//GetDlgItem(IDC_ACCLOSE2B)->EnableWindow(true);
	//GetDlgItem(IDC_M1LISB)->EnableWindow(true);
	//GetDlgItem(IDC_M1LISCLOB)->EnableWindow(true);
	GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(false);

	CloseAudioSocketSend();
	CloseAudioSocketReceive();
	CloseVideoSend();
	CloseVideoReceive();
	CloseVideoSocketSend();
	CloseVideoSocketReceive();

}

//void CMenu1::OnBnClickedRadio14()
//{
//	m_TraType=0;
//}
//
//void CMenu1::OnBnClickedRadio15()
//{
//	m_TraType=1;
//}
//
//void CMenu1::OnBnClickedRadio21()
//{
//	m_TraType=2;
//}
//
//
//void CMenu1::OnBnClickedAmrrate1()
//{
//My_Mode=MR475;
//}
//
//void CMenu1::OnBnClickedAmrrate2()
//{
//	My_Mode=MR515;
//}
//
//void CMenu1::OnBnClickedAmrrate3()
//{
//	
//	My_Mode=MR59;
//}
//
//void CMenu1::OnBnClickedAmrrate4()
//{
//	
//	My_Mode=MR67;
//}
//
//void CMenu1::OnBnClickedAmrrate5()
//{
//	
//	My_Mode=MR795;
//}

//void CMenu1::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	CRect rectL;
//	GetDlgItem(IDC_STATICV1)->GetWindowRect(&rectL);//获取控件相对于屏幕的位置	
//	ScreenToClient(rectL);//转化为对话框上的相对位置
//	rectL.top += 20;
//	rectL.bottom -= 0;
//	rectL.left +=0;
//	rectL.right -=0 ;
//CPaintDC dc(this);
// CBitmap   bitmap;
// bitmap.LoadBitmap(IDB_BITMAP6);    //这个IDB_BITMAP1要自己添加
// CBrush   brush;
// brush.CreatePatternBrush(&bitmap);
// CBrush*   pOldBrush   =   dc.SelectObject(&brush);
// dc.Rectangle(rectL.left,rectL.top,rectL.right,rectL.bottom);   // 这些参数可以调整图片添加位置和大小
// dc.SelectObject(pOldBrush);
//}


void CMenu1::OnBnClickedBvcall()
{	GetDlgItem(IDC_BVCALL)->EnableWindow(false);
	OpenVideoSend();
	OpenVideoReceive();
	OpenVideoSocketSend();
	OpenVideoSocketReceive();
}

void CMenu1::OnBnClickedBinit()
{
	GetDlgItem(IDC_BCALLCLOSE)->EnableWindow(true);
	GetDlgItem(IDC_BINIT)->EnableWindow(false);
	//GetDlgItem(IDC_M1LISB)->EnableWindow(false);
	GetDlgItem(IDC_BVCALL)->EnableWindow(true);
	GetDlgItem(IDC_BACALL)->EnableWindow(true);
//********************LDS 配置远端地址端口
	Is_Call=1;
	UpdateData(true);
	LPTSTR  m_M1SIPc=(LPTSTR)(LPCTSTR) m_M1SIP;
	ConfigSGWIPandPort( m_M1SIPc,m_M1SAPort,m_M1SVPort);//172.16.0.117

//********************LDS配置视频播放

//LDS 配置播放窗口
	CWnd *pWndStatev1 = NULL;  
	pWndStatev1 = GetDlgItem(IDC_PIC1);
	CRect rectL;
	pWndStatev1->GetWindowRect(&rectL);//获取控件相对于屏幕的位置	
//********获取本地窗口
	ConfigVideoDisplayerSet(0, rectL.left,rectL.top,rectL.right-rectL.left, rectL.bottom-rectL.top);
	ConfigVideoDisplayerSetA(0, rectL.left,rectL.top,rectL.right-rectL.left, rectL.bottom-rectL.top);
//LDS 配置视频播放
	ConfigLOCALIPID( 10, 30 );
	ConfigDESTIPID( 10, 30 );
	SystemInti();
	
//********************LDS配置音频播放
	Acess_Num=1;//LDS添加为判断哪条等待线路接入
	OpenAudioSend();
	OpenAudioReceive1();

}

//void CMenu1::OnBnClickedVc1b()
//{		
//	LPTSTR m_M1LIP1c=(LPTSTR)(LPCTSTR)M4IP1;
//	m_M1LPort1s=(short)atoi(M4PORT1)-1;
//	ConfigSGWIPandPort( m_M1LIP1c,m_M1LPort1s,m_M1LPort1s+2);
////lds添加for视频播放*******************
//	CWnd *pWndStatev1 = NULL;  
//	pWndStatev1 = GetDlgItem(IDB_PIC1);
//	CRect rectL;
//	pWndStatev1->GetWindowRect(&rectL);//获取控件相对于屏幕的位置	
//	ScreenToClient(rectL);//转化为对话框上的相对位置
// //*******************结束
//	ConfigVideoDisplayerSet(rectL.left, rectL.top, 0, 176, 144);
//    ConfigVideoDisplayerSetA(0, 177,0 , 176, 144);
//	ConfigLOCALIPID( 10, 30 );
//	ConfigDESTIPID( 10, 30 );
//	SystemInti();
//	OpenVideoSocketSend();
//	OpenVideoSocketReceive();
//}
//
//void CMenu1::OnBnClickedVc2b()
//{	LPTSTR m_M1LIP2c=(LPTSTR)(LPCTSTR)M4IP2;
//	m_M1LPort2s=(short)atoi(M4PORT2)-1;
//	ConfigSGWIPandPort( m_M1LIP2c,m_M1LPort2s,m_M1LPort2s+2);
//	SystemInti();
//	OpenVideoSocketSend();
//	OpenVideoSocketReceive();
//}
//
//void CMenu1::OnBnClickedRadio23()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CMenu1::OnPaint()
{
		 CPaintDC   dc(this);   
         // CRect   rect;   
         // GetClientRect(&rect);   
         // CDC   dcMem;   
         // dcMem.CreateCompatibleDC(&dc);   
         // CBitmap   bmpBackground;   
         // bmpBackground.LoadBitmap(IDB_BITMAP10);   
         //         //IDB_BITMAP是你自己的图对应的ID   
         // BITMAP   bitmap;   
         // bmpBackground.GetBitmap(&bitmap);   
         // CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
         // dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         //bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
}

void CMenu1::OnBnClickedButton3()
{
	H_local=1;
}

void CMenu1::OnBnClickedButton11()
{
	H_local=0;
}

//void CMenu1::OnBnClickedStaticL()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

