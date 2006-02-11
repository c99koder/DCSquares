// DCSquares-MFC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <winsock.h>
#include "DCSquares-MFC.h"
#include "MainFrm.h"

#include "UpdateDlg.h"
#include "HighScoreDlg.h"
#include "ChatDlg.h"
#include "GeneralSettingsPage.h"
#include "SquarenetSettingsPage.h"
#include "UpdateSettingsPage.h"
#include "DCSquares-MFCDoc.h"
#include "DCSquares-MFCView.h"
#include "ul.h"

#include <dxerr9.h>
#include "dxstdafx.h"

#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "hyperlink.h"
#include ".\dcsquares-mfc.h"

#include "theme.h"
#include "text.h"
#include "level.h"

void texture_init();

// CDCSquaresMFCApp

BEGIN_MESSAGE_MAP(CDCSquaresMFCApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_CHECK_UPDATES, OnCheckUpdates)
	ON_COMMAND(ID_PREFS, OnPrefs)
	ON_COMMAND(ID_SCORES_HIGHSCORES, OnScoresHighscores)
	ON_COMMAND(ID_HELP_HOWTOPLAY, OnHelpHowtoplay)
	ON_COMMAND(ID_GAME_CONNECTTOLOBBY32778, OnGameConnecttolobby)
END_MESSAGE_MAP()


// CDCSquaresMFCApp construction

CSoundManager *g_pSoundManager = NULL;

extern char dcs_theme[100];

CDCSquaresMFCApp::CDCSquaresMFCApp()
: sfx(0)
, bgm(0)
, autoSubmit(FALSE)
, updateMode(0)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	CoInitialize(NULL);
	srand(time(NULL));
}


// The one and only CDCSquaresMFCApp object

CDCSquaresMFCApp theApp;

// CDCSquaresMFCApp initialization

BOOL CDCSquaresMFCApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Sam Steele"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDCSquaresMFCDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDCSquaresMFCView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	m_pMainWnd = pDocTemplate->CreateNewFrame(pDocTemplate->CreateNewDocument(),NULL);
	
	//m_pMainWnd->ShowWindow(SW_HIDE);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	LoadSettings();

	texture_init();
	levels_init();

	WORD wVersionRequested;
WSADATA wsaData;
int err;
 
wVersionRequested = MAKEWORD( 2, 2 );
 
err = WSAStartup( wVersionRequested, &wsaData);
AfxInitRichEdit();
HRESULT hr;
    g_pSoundManager = new CSoundManager();
    if( NULL == g_pSoundManager )
    {
        DXTRACE_ERR_MSGBOX( TEXT("Initialize"), E_OUTOFMEMORY );
        return FALSE;
    }
	/*if( FAILED( hr = g_pSoundManager->Initialize( m_pMainWnd->GetSafeHwnd(), DSSCL_PRIORITY ) ) )
    {
        DXTRACE_ERR_MSGBOX( TEXT("Initialize"), hr );
        return FALSE;
    }
    
    if( FAILED( hr = g_pSoundManager->SetPrimaryBufferFormat( 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR_MSGBOX( TEXT("SetPrimaryBufferFormat"), hr );
        return FALSE;
    }*/
		
	if(text_init("VeraBd.txf",20)==0 && load_theme((char *)theme.GetString(),sfx)==0) {
		// The one and only window has been initialized, so show and update it
		//m_pMainWnd->UpdateWindow();
		//m_pMainWnd->RedrawWindow();
		statusDlg.Create(IDD_STATUS);
		statusDlg.CenterWindow();
		updateDlg.autoUpdate();
		m_pMainWnd->ShowWindow(SW_SHOW);
	} else {
		return FALSE;
	}
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CHyperLink dcsurl;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DCS_URL, dcsurl);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// App command to run the dialog
void CDCSquaresMFCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDCSquaresMFCApp message handlers


int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
		return 0;
}

DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psBuffer = (CString *)dwCookie;

	if (cb >= psBuffer->GetLength())
		cb = psBuffer->GetLength();

	for (int i=0;i<cb;i++)
	{
		*(pbBuff+i) = psBuffer->GetAt(i);
	}

	*pcb = cb;

	*psBuffer = psBuffer->Mid(cb);

	return 0;
}

void CDCSquaresMFCApp::OnCheckUpdates()
{
	updateDlg.checkUpdates();
}

void CDCSquaresMFCApp::OnPrefs()
{
	CGeneralSettingsPage generalPage;
	CSquarenetSettingsPage squarenetPage;
	CUpdateSettingsPage updatePage;
	CPropertySheet dlgSheet("Settings");

	generalPage.soundCheck=sfx;
	generalPage.musicCheck=bgm;
	squarenetPage.usernameTxt=username;
	squarenetPage.passwordTxt=password;
	squarenetPage.autoCheck=autoSubmit;

	dlgSheet.AddPage(&generalPage);
	dlgSheet.AddPage(&squarenetPage);
	dlgSheet.AddPage(&updatePage);

	dlgSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	if(dlgSheet.DoModal()==IDOK) {
		statusDlg.statusTxt.SetWindowText("Applying settings");
		statusDlg.ShowWindow(SW_SHOW);
		statusDlg.UpdateWindow();
		theme=generalPage.themeTxt;
		sfx=generalPage.soundCheck;
		bgm=generalPage.musicCheck;
		username=squarenetPage.usernameTxt;
		password=squarenetPage.passwordTxt;
		autoSubmit=squarenetPage.autoCheck;
		SaveSettings();
		unload_theme();
		mp3player.hrStop();
		load_theme((char *)theme.GetString(),sfx);
		statusDlg.ShowWindow(SW_HIDE);
	}
}
void CDCSquaresMFCApp::SaveSettings(void)
{	
	WriteProfileString("Settings","theme",theme);
	WriteProfileInt("Settings","music",bgm);
	WriteProfileInt("Settings","sound",sfx);
	WriteProfileString("Settings","username",username);
	WriteProfileString("Settings","password",password);
	WriteProfileInt("Settings","autoSubmit",autoSubmit);
	WriteProfileInt("Settings","updateMode",updateMode);
}

void CDCSquaresMFCApp::LoadSettings(void)
{
	theme=GetProfileString("Settings","theme","Default");
	bgm=GetProfileInt("Settings","music",1);
	sfx=GetProfileInt("Settings","sound",1);
	username=GetProfileString("Settings","username");
	password=GetProfileString("Settings","password");
	autoSubmit=GetProfileInt("Settings","autoSubmit",0);
	updateMode=GetProfileInt("Settings","updateMode",0);
	strcpy(dcs_theme,(char *)theme.GetString());
}

void CDCSquaresMFCApp::OnScoresHighscores()
{
	// TODO: Add your command handler code here
	CHighScoreDlg hs;
	hs.DoModal();
}

void CDCSquaresMFCApp::OnHelpHowtoplay()
{
	// TODO: Add your command handler code here
	CHyperLink::GotoURL("http://dcsquares.c99.org/howtoplay.php",0);
}

void CDCSquaresMFCApp::OnGameConnecttolobby()
{
	// TODO: Add your command handler code here
	CChatDlg c;
	c.DoModal();
}
