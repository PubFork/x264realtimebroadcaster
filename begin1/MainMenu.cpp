// MainMenu.cpp : 实现文件
//

#include "stdafx.h"
#include "begin1.h"
#include "MainMenu.h"
#include "begin1Dlg.h"
#include "dll\GlobalVariable.h"
// CMainMenu 对话框

IMPLEMENT_DYNAMIC(CMainMenu, CDialog)

CMainMenu::CMainMenu(CWnd* pParent /*=NULL*/)
: CDialog(CMainMenu::IDD, pParent)
{

}

CMainMenu::~CMainMenu()
{
}

void CMainMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
}


BEGIN_MESSAGE_MAP(CMainMenu, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainMenu::OnTcnSelchangeTab1)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SHOWTASK2,OnShowTask)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMainMenu 消息处理程序

BOOL CMainMenu::OnInitDialog()
{
	CDialog::OnInitDialog();
	pDlgMM = this;//保存当前对话框指针
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
	SetWindowPos(&wndTopMost, 00, 00, 430, 600, SWP_ASYNCWINDOWPOS);
	cfont.CreateFont(25,        //设置静态对话框的字体大小
		0, 0, 0, FW_NORMAL,			//设置字体颜射
		FALSE, FALSE,
		0,
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, _T("Lucida Console")); //设置字体Ariel
	CWnd*  pWndS = GetDlgItem(IDC_STATIC);
	pWndS->SetFont(&cfont);        //使设置运用


	m_tabctrl.InsertItem(0, _T("呼叫窗口"));      //添加参数一选项卡 
 
	m_menu1.Create(IDD_MENU1,pDlgMM);//关联，极其重要 


	//获得IDC_TABTEST客户区大小

	CRect rs;
	GetClientRect(&rs);


	//调整子对话框在父窗口中的位置
	rs.top += 100;
	rs.bottom -= 20;
	rs.left += 28;
	rs.right -= 2;
	//设置子对话框尺寸并移动到指定位置
	m_menu1.MoveWindow(&rs);



	//分别设置隐藏和显示
	m_tabctrl.SetCurSel(0);
	m_menu1.ShowWindow(true);




	//设置默认的选项卡

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMainMenu::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//根据TAB的变化改变界面
	int CurSel = m_tabctrl.GetCurSel();
	tab_Mode=CurSel;
	switch(CurSel)
	{
	case 0:
		m_menu1.ShowWindow(true);



		break;

	default:

		*pResult = 0;
	}
}

HBRUSH CMainMenu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID()==IDC_STATIC)
	{
		pDC->SetTextColor(RGB(100,10,100));
	} // TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CMainMenu::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*if (nID == SC_MOVE || nID==0xF012)
	return;
	if(nID==SC_MINIMIZE)
	{ShowWindow(SW_HIDE);
	 }
	else
	{
		CDialog::OnSysCommand(nID, lParam);

	}*/
	CDialog::OnSysCommand(nID, lParam);
}
LRESULT CMainMenu::OnShowTask(WPARAM wParam, LPARAM lParam)
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
			this->ShowWindow(SW_SHOWNORMAL); // 显示主窗口
			DeleteTray();
		}
		break;
	}
	return 0;
}
void CMainMenu::ToTray()

{

	NOTIFYICONDATA m_nid;    //NOTIFYICONDATA结构包含了系统用来处理托盘图标的信息，

	//它包括选择的图标、回调消息、提示消息和图标对应的窗口等内容。

	m_nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);         //以字节为单位的这个结构的大小

	m_nid.hWnd=this->m_hWnd;          //接收托盘图标通知消息的窗口句柄

	m_nid.uID=IDR_MAINFRAME;          //应用程序定义的该图标的ID号

	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;         //设置该图标的属性

	m_nid.uCallbackMessage=WM_SHOWTASK2;             //应用程序定义的消息ID号，此消息传递给hWnd  

	m_nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   //图标的句柄

	strcpy(m_nid.szTip,_T("R 2.0调度台"));    //鼠标停留在图标上显示的提示信息 

	Shell_NotifyIcon(NIM_ADD,&m_nid);    //在托盘区添加图标de函数 ,函数说明见文章附录

	ShowWindow(SW_HIDE);    //隐藏主窗口
	temp_tab_Mode=tab_Mode;
	tab_Mode=6;

}

void CMainMenu::DeleteTray()
{
//	m_nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);  
//	m_nid.hWnd=this->m_hWnd;  
//	m_nid.uID=IDR_MAINFRAME;  
//	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
//	m_nid.uCallbackMessage=WM_SHOWTASK2;//自定义的消息名称
//	m_nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));  
//	wcscpy(m_nid.szTip,_T("程序名称"));    //信息提示条为“计划任务提醒”  
	Shell_NotifyIcon(NIM_DELETE,&m_nid);    //在托盘区删除图标
tab_Mode=temp_tab_Mode;
	temp_tab_Mode=0;
}
//void CMainMenu::OnOK()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	//CDialog::OnOK();避免回车删除
//}

BOOL CMainMenu::PreTranslateMessage(MSG* pMsg)
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

void CMainMenu::OnPaint()
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
