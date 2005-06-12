// ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "ChatDlg.h"
#include ".\chatdlg.h"
#include "net.h"

// CChatDlg dialog

IMPLEMENT_DYNAMIC(CChatDlg, CDialog)
CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERLIST, lstUserList);
	DDX_Control(pDX, IDC_CHATTEXT, txtChatText);
	DDX_Control(pDX, IDC_CHATINPUT, txtChatInput);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNSEND, OnBtnSend)
END_MESSAGE_MAP()


// CChatDlg message handlers
CListBox *os_user_list;
CEdit *os_chat_text;

void os_chat_reload_users() {
	USES_CONVERSION;
	struct userlist_node *current=get_userlist();

	os_user_list->ResetContent();
	while(current!=NULL) {
		os_user_list->AddString(current->username);
		current=current->next;
	}
}

void os_chat_insert_text(char *text) {
	CString s;
	
	os_chat_text->GetWindowText(s);
	s += text;
	s += "\r\n";
	os_chat_text->SetWindowText(s);
	os_chat_text->LineScroll(os_chat_text->GetLineCount());
}

int CChatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	os_user_list = &lstUserList;
	os_chat_text = &txtChatText;

	lobby_connect("192.168.11.101","DJ Shibby","jay123");
	SetTimer(1000,1000 / 10,NULL);
	return 0;
}

void CChatDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	lobby_update();
	CDialog::OnTimer(nIDEvent);
}

void CChatDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	lobby_disconnect();
	CDialog::OnClose();
}

void CChatDlg::OnBtnSend()
{

	// TODO: Add your control notification handler code here
	CString s;
	txtChatInput.GetWindowText(s);
	lobby_send(1,1,(char *)(LPCTSTR)s);
	txtChatInput.SetWindowText("");
	txtChatInput.SetFocus();
}
