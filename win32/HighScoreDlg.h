#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHighScoreDlg dialog

class CHighScoreDlg : public CDialog
{
	DECLARE_DYNAMIC(CHighScoreDlg)

public:
	CHighScoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHighScoreDlg();

// Dialog Data
	enum { IDD = IDD_HIGHSCORES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListCtrl ScoreList;
	BOOL OnInitDialog();
};
