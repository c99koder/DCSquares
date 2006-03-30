// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "StatusDlg.h"


// CStatusDlg dialog

IMPLEMENT_DYNAMIC(CStatusDlg, CDialog)
CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDlg::IDD, pParent)
{
}

CStatusDlg::~CStatusDlg()
{
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUSMSG, statusTxt);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)
END_MESSAGE_MAP()


// CStatusDlg message handlers
