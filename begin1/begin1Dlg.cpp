// begin1Dlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// Cbegin1Dlg �Ի���




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


// Cbegin1Dlg ��Ϣ�������

BOOL Cbegin1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)|WS_MINIMIZEBOX);//��С����ť
	pDlgbD=this;//���汾ʵ��ָ��
	CString   str;//Ϊ��ʾIP�����ַ���    
	IPnum="127.0.0.1";
	UpdateData(false);
	WSADATA wsData;  
	::WSAStartup(MAKEWORD(2,2), &wsData); 

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ȡ����IP
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
	{temp=str.Right(lenth-i-1); //����1 Ϊ��ȥ���ո�
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cbegin1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
                  //IDB_BITMAP�����Լ���ͼ��Ӧ��ID   
          BITMAP   bitmap;   
          bmpBackground.GetBitmap(&bitmap);   
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);   
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
	DeleteDC(dcMem);
	//free(pbmpOld);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	//{AfxMessageBox("�������!");
	//}
// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void Cbegin1Dlg::OnBnClickedButton2()
{

	OnCancel();// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//ȡֵ// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


LRESULT Cbegin1Dlg::OnShowTask(WPARAM wParam, LPARAM lParam)
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
			this->ShowWindow(SW_SHOWNORMAL);
			DeleteTray();// ��ʾ������
		}
		break;
	}
	return 0;
}
void Cbegin1Dlg::ToTray()

{

	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);         //���ֽ�Ϊ��λ������ṹ�Ĵ�С

	nid.hWnd=this->m_hWnd;          //��������ͼ��֪ͨ��Ϣ�Ĵ��ھ��

	nid.uID=IDR_MAINFRAME;          //Ӧ�ó�����ĸ�ͼ���ID��

	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;         //���ø�ͼ�������

	nid.uCallbackMessage=WM_SHOWTASK2;             //Ӧ�ó��������ϢID�ţ�����Ϣ���ݸ�hWnd  

	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   //ͼ��ľ��

	strcpy(nid.szTip,_T("R2.0����̨"));    //���ͣ����ͼ������ʾ����ʾ��Ϣ 

	Shell_NotifyIcon(NIM_ADD,&nid);    //�����������ͼ��de���� ,����˵�������¸�¼

	ShowWindow(SW_HIDE);    //����������

}

void Cbegin1Dlg::DeleteTray()
{
	Shell_NotifyIcon(NIM_DELETE,&nid);    //��������ɾ��ͼ��
}
//void Cbegin1Dlg::OnOK()
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	//CDialog::OnOK();
//}

BOOL Cbegin1Dlg::PreTranslateMessage(MSG* pMsg)
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

