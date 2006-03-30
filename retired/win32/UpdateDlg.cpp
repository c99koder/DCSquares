// UpdateDlg.cpp : implementation file
//

#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "UpdateDlg.h"
#include "hyperlink.h"
#include ".\updatedlg.h"
#include <winsock.h>
#include "http.h"
// CUpdateDlg dialog

IMPLEMENT_DYNAMIC(CUpdateDlg, CDialog)
CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
{
}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT22, updateTxt);
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUpdateDlg message handlers

void CUpdateDlg::checkUpdates(void)
{
	char ct[100];
	int len=0;
    char *buf;
	CFile fileUpdate;
	FILE *f;
	theApp.statusDlg.statusTxt.SetWindowText("Checking for updates");
	theApp.statusDlg.ShowWindow(SW_SHOW);
	theApp.statusDlg.UpdateWindow();
	http_get_file("DCS_LATEST","dcsquares.c99.org",80,"/updates/LATEST-WIN",ct,&len);
	f=fopen("DCS_LATEST","r");
	if(!f) return;
	fgets(ct,100,f);
	fclose(f);
	unlink("DCS_LATEST");
	printf("%s\n",ct);
	theApp.statusDlg.ShowWindow(SW_HIDE);
	if(strcmp("2.0.0\n",ct)) {
		http_get_file("DCS_CHANGES","dcsquares.c99.org",80,"/updates/CHANGES-WIN.rtf",ct,&len);
		fileUpdate.Open("DCS_CHANGES",CFile::modeRead);
		buf=(char *)malloc(fileUpdate.GetLength()+2);
		fileUpdate.Read(buf,fileUpdate.GetLength());
		buf[fileUpdate.GetLength()]='\0';
		fileUpdate.Close();
		unlink("DCS_CHANGES");
	// TODO: Add your control notification handler code here
		sWriteText=buf;
		free(buf);
		CTime t1(CTime::GetCurrentTime());
		theApp.WriteProfileInt("","lastUpdateCheck",t1.GetTime());
		DoModal();
	}
}

void CUpdateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
		// This is hard-coded for example purposes. It is likely this would
		// be read from file or another source.
		EDITSTREAM es;
		es.dwCookie = (DWORD)&sWriteText; // Pass a pointer to the CString to the callback function
		es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function
		updateTxt.SetTargetDevice(NULL,0);
		updateTxt.StreamIn(SF_RTF,es); // Perform the streaming}
}

void CUpdateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CHyperLink::GotoURL("http://dcsquares.c99.org/",0);
	OnOK();
}

void CUpdateDlg::autoUpdate(void)
{
	CTime t1;
	CTime t2;
	__time64_t lastChecked;

	if(theApp.updateMode==0) return;
	t1=CTime::GetCurrentTime();
	lastChecked=theApp.GetProfileInt("","lastUpdateCheck",0);
	if(lastChecked!=0 && theApp.updateMode==1) t2=CTime(lastChecked) + CTimeSpan(1,0,0,0);
	if(lastChecked!=0 && theApp.updateMode==2) t2=CTime(lastChecked) + CTimeSpan(7,0,0,0);
	if(lastChecked!=0 && theApp.updateMode==3) t2=CTime(lastChecked) + CTimeSpan(30,0,0,0);
	if(t1==0 || t1>t2) checkUpdates();
}
