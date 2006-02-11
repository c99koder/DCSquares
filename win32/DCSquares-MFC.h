// DCSquares-MFC.h : main header file for the DCSquares-MFC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "statusdlg.h"
#include "updatedlg.h"
#include "PlayerMP3.h"

// CDCSquaresMFCApp:
// See DCSquares-MFC.cpp for the implementation of this class
//

class CDCSquaresMFCApp : public CWinApp
{
public:
	CDCSquaresMFCApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckUpdates();
	afx_msg void OnPrefs();

public:
	BOOL sfx;
	BOOL bgm;
	CString theme;
	CString username;
	CString password;
	void SaveSettings(void);
	void LoadSettings(void);
	afx_msg void OnScoresHighscores();
	BOOL autoSubmit;
	CStatusDlg statusDlg;
	CPlayerMP3 mp3player;
	CUpdateDlg updateDlg;
	int updateMode;
	afx_msg void OnHelpHowtoplay();
	afx_msg void OnGameConnecttolobby();
};
DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
extern CDCSquaresMFCApp theApp;