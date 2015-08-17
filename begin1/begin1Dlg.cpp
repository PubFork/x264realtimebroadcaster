// begin1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "begin1.h"
#include "begin1Dlg.h"
#include "MainMenu.h"
#include "stdlib.h"
#include <Winsock2.h>  
#include <windows.h>  
#include "MenuGlobal.h"
#pragma comment(lib, "Ws2_32")  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cbegin1Dlg 对话框




Cbegin1Dlg::Cbegin1Dlg(CWnd* pParent /*=NULL*/)
: CDialog(Cbegin1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cbegin1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, IPnum);
	DDX_Text(pDX, IDC_EDIT3, m_passkey);
}

BEGIN_MESSAGE_MAP(Cbegin1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &Cbegin1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cbegin1Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &Cbegin1Dlg::OnEnChangeEdit2)
	ON_MESSAGE(WM_SHOWTASK2,OnShowTask)
	ON_EN_CHANGE(IDC_IPH, &Cbegin1Dlg::OnEnChangeIph)
END_MESSAGE_MAP()


// Cbegin1Dlg 消息处理程序

BOOL Cbegin1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|WS_MINIMIZEBOX);//最小化按钮
	pDlgbD=this;//保存本实例指针
	CString   str;//为显示IP定义字符串    
	IPnum="127.0.0.1";
	UpdateData(false);
	WSADATA wsData;  
	::WSAStartup(MAKEWORD(2,2), &wsData); 

	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//获取本地IP
	char   szHostName[200];                     
	if( gethostname(szHostName,200)==0)           
	{    
		struct   hostent   *   pHost;             
		int   i;                       
		pHost   =   gethostbyname(szHostName);                       
		for(   i   =   0;   pHost!=   NULL   &&   pHost-> h_addr_list[i]!=   NULL;   i++   )             
		{             

			int   j;                       
			for(   j   =   0;   j   <   pHost-> h_length;   j++   )             
			{             
				CString   addr;                       
				if(   j   >   0   )             
					str   +=   ". ";                       
				addr.Format( _T("%u "),   (unsigned   int)((unsigned   char*)pHost-> h_addr_list[i])[j]); 
				str   +=   addr;             
			}   
		}	}
		int i=1;
	CString temp;
	char temp1=str.GetAt(i-1);
	char temp2=str.GetAt(i);
	char temp3=str.GetAt(i+1);
	int lenth=str.GetLength();

	while(1)
	{if(i<=lenth-5)
	{if(temp2==' '&&temp3!='.'&&temp1!='.')
	{temp=str.Right(lenth-i-1); //减掉1 为了去掉空格
	str=temp;
	i=1;
	lenth=str.GetLength();
	temp1=str.GetAt(i-1);
	temp2=str.GetAt(i);
	temp3=str.GetAt(i+1);
	}
	else 
	{i++;
	temp1=str.GetAt(i-1);
	temp2=str.GetAt(i);
	temp3=str.GetAt(i+1);}
	}
	else
	{break;}
	}

	IPnum=str;
	CEdit* pBoxThree;
	pBoxThree = (CEdit*)GetDlgItem(IDC_IPH);
	pBoxThree->SetWindowText(str);
	LocalIP=IPnum;
	ShowWindow(SW_HIDE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cbegin1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	if(nID==SC_MINIMIZE)

		pDlgbD->ShowWindow(0);
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cbegin1Dlg::OnPaint()
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
	//	CDialog::OnPaint();
		 CPaintDC   dc(this);   
          CRect   rect;   
          GetClientRect(&rect);   
          CDC   dcMem;   
          dcMem.CreateCompatibleDC(&dc);   
          CBitmap   bmpBackground;   
          bmpBackground.LoadBitmap(IDB_BITMAP10);   
                  //IDB_BITMAP是你自己的图对应的ID   
          BITMAP   bitmap;   
          bmpBackground.GetBitmap(&bitmap);   
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
	DeleteDC(dcMem);
	//free(pbmpOld);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cbegin1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cbegin1Dlg::OnBnClickedButton1()
{
		UpdateData(true);
	/*if(m_passkey=="123")
	{*/
	pDlgbD->ShowWindow(false); 
	CMainMenu dlg1;
	dlg1.DoModal();
	OnCancel();/*}*/
	//else
	//{AfxMessageBox("密码错误!");
	//}
// TODO: 在此添加控件通知处理程序代码
}

void Cbegin1Dlg::OnBnClickedButton2()
{

	OnCancel();// TODO: 在此添加控件通知处理程序代码
}

void Cbegin1Dlg::OnEnChangeEdit2()
{
	CEdit* pBoxOne;
	CEdit* pBoxTwo;
	CString str;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT2);
	pBoxTwo = (CEdit*)GetDlgItem(IDC_EDIT4);
	pBoxOne->GetWindowText(str);
	pBoxTwo->SetWindowText(str);
	//取值// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


LRESULT Cbegin1Dlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME) return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP: // 右键起来时弹出菜单
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint); // 得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();// 声明一个弹出式菜单
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y, this);
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
	case WM_LBUTTONDBLCLK: // 双击左键的处理
		{
			this->ShowWindow(SW_SHOWNORMAL);
			DeleteTray();// 显示主窗口
		}
		break;
	}
	return 0;
}
void Cbegin1Dlg::ToTray()

{

	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);         //以字节为单位的这个结构的大小

	nid.hWnd=this->m_hWnd;          //接收托盘图标通知消息的窗口句柄

	nid.uID=IDR_MAINFRAME;          //应用程序定义的该图标的ID号

	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;         //设置该图标的属性

	nid.uCallbackMessage=WM_SHOWTASK2;             //应用程序定义的消息ID号，此消息传递给hWnd  

	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   //图标的句柄

	strcpy(nid.szTip,_T("R2.0调度台"));    //鼠标停留在图标上显示的提示信息 

	Shell_NotifyIcon(NIM_ADD,&nid);    //在托盘区添加图标de函数 ,函数说明见文章附录

	ShowWindow(SW_HIDE);    //隐藏主窗口

}

void Cbegin1Dlg::DeleteTray()
{
	Shell_NotifyIcon(NIM_DELETE,&nid);    //在托盘区删除图标
}
//void Cbegin1Dlg::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	//CDialog::OnOK();
//}

BOOL Cbegin1Dlg::PreTranslateMessage(MSG* pMsg)
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

