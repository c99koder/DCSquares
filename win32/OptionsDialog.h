#pragma once
#include "afxwin.h"


// COptionsDialog dialog

class COptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(COptionsDialog)

public:
	COptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton BGM;
	CButton SFX;
	CEdit usernameTxt;
	CEdit passwordTxt;
	CButton AutoSubmit;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CComboBox ThemeList;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
};
