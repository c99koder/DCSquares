// SquarenetSettingsPage.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "SquarenetSettingsPage.h"


// CSquarenetSettingsPage dialog

IMPLEMENT_DYNAMIC(CSquarenetSettingsPage, CPropertyPage)
CSquarenetSettingsPage::CSquarenetSettingsPage()
	: CPropertyPage(CSquarenetSettingsPage::IDD)
	, usernameTxt(_T(""))
	, passwordTxt(_T(""))
	, autoCheck(FALSE)
{
}

CSquarenetSettingsPage::~CSquarenetSettingsPage()
{
}

void CSquarenetSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, usernameTxt);
	DDX_Text(pDX, IDC_PASSWORD, passwordTxt);
	DDX_Check(pDX, IDC_AUTOSUBMIT, autoCheck);
}


BEGIN_MESSAGE_MAP(CSquarenetSettingsPage, CPropertyPage)
END_MESSAGE_MAP()


// CSquarenetSettingsPage message handlers
