#pragma once
#include "afxwin.h"


// CUpdateSettingsPage dialog

class CUpdateSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUpdateSettingsPage)

public:
	CUpdateSettingsPage();
	virtual ~CUpdateSettingsPage();

// Dialog Data
	enum { IDD = IDD_SETTINGS_UPDATES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox updateList;
	afx_msg void OnCbnSelchangeUpdatelist();
	BOOL OnInitDialog(void);
};
