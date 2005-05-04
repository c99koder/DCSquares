// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "OptionsDialog.h"
#include ".\optionsdialog.h"


// COptionsDialog dialog

IMPLEMENT_DYNAMIC(COptionsDialog, CDialog)
COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
}

COptionsDialog::~COptionsDialog()
{
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BGM, BGM);
	DDX_Control(pDX, IDC_SFX, SFX);
	DDX_Control(pDX, IDC_USERNAME, usernameTxt);
	DDX_Control(pDX, IDC_PASSWORD, passwordTxt);
	DDX_Control(pDX, IDC_AUTOSUBMIT, AutoSubmit);
	DDX_Control(pDX, IDC_THEMELIST, ThemeList);
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// COptionsDialog message handlers

int COptionsDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}

void COptionsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

	usernameTxt.SetWindowText(theApp.username);
	passwordTxt.SetWindowText(theApp.password);
}

void COptionsDialog::OnBnClickedOk()
{

	OnOK();
}
