#pragma once
#include "afxwin.h"


// CSquarenetSettingsPage dialog

class CSquarenetSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSquarenetSettingsPage)

public:
	CSquarenetSettingsPage();
	virtual ~CSquarenetSettingsPage();

// Dialog Data
	enum { IDD = IDD_SETTINGS_SQUARENET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString usernameTxt;
	CString passwordTxt;
	BOOL autoCheck;
};
