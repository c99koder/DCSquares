// UpdateSettingsPage.cpp : implementation file
//

#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "UpdateSettingsPage.h"
#include ".\updatesettingspage.h"


// CUpdateSettingsPage dialog

IMPLEMENT_DYNAMIC(CUpdateSettingsPage, CPropertyPage)
CUpdateSettingsPage::CUpdateSettingsPage()
	: CPropertyPage(CUpdateSettingsPage::IDD)
{
}

CUpdateSettingsPage::~CUpdateSettingsPage()
{
}

void CUpdateSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPDATELIST, updateList);
}


BEGIN_MESSAGE_MAP(CUpdateSettingsPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_UPDATELIST, OnCbnSelchangeUpdatelist)
END_MESSAGE_MAP()


// CUpdateSettingsPage message handlers

void CUpdateSettingsPage::OnCbnSelchangeUpdatelist()
{
	// TODO: Add your control notification handler code here
	theApp.updateMode=updateList.GetCurSel();
}

BOOL CUpdateSettingsPage::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	updateList.SetCurSel(theApp.updateMode);
	return 0;
}
