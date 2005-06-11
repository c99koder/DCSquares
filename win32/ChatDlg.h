#pragma once
#include "afxwin.h"


// CChatDlg dialog

class CChatDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChatDlg();

// Dialog Data
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListBox lstUserList;
	afx_msg void OnTimer(UINT nIDEvent);
	CEdit txtChatText;
	afx_msg void OnClose();
};
