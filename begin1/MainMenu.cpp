// MainMenu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "begin1.h"
#include "MainMenu.h"
#include "begin1Dlg.h"
#include "dll\GlobalVariable.h"
// CMainMenu �Ի���

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


// CMainMenu ��Ϣ�������

BOOL CMainMenu::OnInitDialog()
{
	CDialog::OnInitDialog();
	pDlgMM = this;//���浱ǰ�Ի���ָ��
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
	SetWindowPos(&wndTopMost, 00, 00, 430, 600, SWP_ASYNCWINDOWPOS);
	cfont.CreateFont(25,        //���þ�̬�Ի���������С
		0, 0, 0, FW_NORMAL,			//������������
		FALSE, FALSE,
		0,
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS, _T("Lucida Console")); //��������Ariel
	CWnd*  pWndS = GetDlgItem(IDC_STATIC);
	pWndS->SetFont(&cfont);        //ʹ��������


	m_tabctrl.InsertItem(0, _T("���д���"));      //��Ӳ���һѡ� 
 
	m_menu1.Create(IDD_MENU1,pDlgMM);//������������Ҫ 


	//���IDC_TABTEST�ͻ�����С

	CRect rs;
	GetClientRect(&rs);


	//�����ӶԻ����ڸ������е�λ��
	rs.top += 100;
	rs.bottom -= 20;
	rs.left += 28;
	rs.right -= 2;
	//�����ӶԻ���ߴ粢�ƶ���ָ��λ��
	m_menu1.MoveWindow(&rs);



	//�ֱ��������غ���ʾ
	m_tabctrl.SetCurSel(0);
	m_menu1.ShowWindow(true);




	//����Ĭ�ϵ�ѡ�

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMainMenu::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//����TAB�ı仯�ı����
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
	} // TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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
	case WM_RBUTTONUP: // �Ҽ�����ʱ�����˵�
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint); // �õ����λ��
			CMenu menu;
			menu.CreatePopupMenu();// ����һ������ʽ�˵�
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�ر�"));
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y, this);
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
	case WM_LBUTTONDBLCLK: // ˫������Ĵ���
		{
			this->ShowWindow(SW_SHOWNORMAL); // ��ʾ������
			DeleteTray();
		}
		break;
	}
	return 0;
}
void CMainMenu::ToTray()

{

	NOTIFYICONDATA m_nid;    //NOTIFYICONDATA�ṹ������ϵͳ������������ͼ�����Ϣ��

	//������ѡ���ͼ�ꡢ�ص���Ϣ����ʾ��Ϣ��ͼ���Ӧ�Ĵ��ڵ����ݡ�

	m_nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);         //���ֽ�Ϊ��λ������ṹ�Ĵ�С

	m_nid.hWnd=this->m_hWnd;          //��������ͼ��֪ͨ��Ϣ�Ĵ��ھ��

	m_nid.uID=IDR_MAINFRAME;          //Ӧ�ó�����ĸ�ͼ���ID��

	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;         //���ø�ͼ�������

	m_nid.uCallbackMessage=WM_SHOWTASK2;             //Ӧ�ó��������ϢID�ţ�����Ϣ���ݸ�hWnd  

	m_nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   //ͼ��ľ��

	strcpy(m_nid.szTip,_T("R 2.0����̨"));    //���ͣ����ͼ������ʾ����ʾ��Ϣ 

	Shell_NotifyIcon(NIM_ADD,&m_nid);    //�����������ͼ��de���� ,����˵�������¸�¼

	ShowWindow(SW_HIDE);    //����������
	temp_tab_Mode=tab_Mode;
	tab_Mode=6;

}

void CMainMenu::DeleteTray()
{
//	m_nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);  
//	m_nid.hWnd=this->m_hWnd;  
//	m_nid.uID=IDR_MAINFRAME;  
//	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
//	m_nid.uCallbackMessage=WM_SHOWTASK2;//�Զ������Ϣ����
//	m_nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));  
//	wcscpy(m_nid.szTip,_T("��������"));    //��Ϣ��ʾ��Ϊ���ƻ��������ѡ�  
	Shell_NotifyIcon(NIM_DELETE,&m_nid);    //��������ɾ��ͼ��
tab_Mode=temp_tab_Mode;
	temp_tab_Mode=0;
}
//void CMainMenu::OnOK()
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	//CDialog::OnOK();����س�ɾ��
//}

BOOL CMainMenu::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
		switch(pMsg->wParam)
	{case VK_RETURN://�س�
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
         //         //IDB_BITMAP�����Լ���ͼ��Ӧ��ID   
         // BITMAP   bitmap;   
         // bmpBackground.GetBitmap(&bitmap);   
         // CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
         // dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         //bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
}
