#pragma once
#include "afxwin.h"


// CGeneralSettingsPage dialog

class CGeneralSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralSettingsPage)

public:
	CGeneralSettingsPage();
	virtual ~CGeneralSettingsPage();

// Dialog Data
	enum { IDD = IDD_SETTINGS_GENERAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox themeList;
	BOOL OnInitDialog(void);
	BOOL musicCheck;
	BOOL soundCheck;
	CString themeTxt;
};
