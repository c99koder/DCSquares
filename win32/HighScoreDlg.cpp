// HighScoreDlg.cpp : implementation file
//

#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "DCSquares-MFC.h"
#include "HighScoreDlg.h"
#include ".\highscoredlg.h"
#include <winsock.h>
#include "http.h"
#include "ScoresSOAPService.h"

// CHighScoreDlg dialog

IMPLEMENT_DYNAMIC(CHighScoreDlg, CDialog)
CHighScoreDlg::CHighScoreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHighScoreDlg::IDD, pParent)
{
}

CHighScoreDlg::~CHighScoreDlg()
{
}

void CHighScoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCORELIST, ScoreList);
	DDX_Control(pDX, IDC_SCORELIST, ScoreList);
}


BEGIN_MESSAGE_MAP(CHighScoreDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CHighScoreDlg message handlers

void CHighScoreDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here

}

int CHighScoreDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

BOOL CHighScoreDlg::OnInitDialog()
{
	USES_CONVERSION;
	CString tmp;
	int x=0;

	CDialog::OnInitDialog();
	theApp.statusDlg.statusTxt.SetWindowText("Retrieving scores");
	theApp.statusDlg.ShowWindow(SW_SHOW);
	theApp.statusDlg.UpdateWindow();
	
	ScoresService::CScoresService Scores;
	ScoresService::ScoreEntry *entries;
	int size;

	Scores.getScores(L"",L"",L"15",&entries,&size);

	ScoreList.SetExtendedStyle(LVS_EX_GRIDLINES);
	ScoreList.InsertColumn(0,"#",LVCFMT_LEFT,25);
	ScoreList.InsertColumn(1,"Username",LVCFMT_LEFT,80);
	ScoreList.InsertColumn(2,"Combo",LVCFMT_CENTER,50);
	ScoreList.InsertColumn(3,"Time",LVCFMT_CENTER,50);
	ScoreList.InsertColumn(4,"Score",LVCFMT_RIGHT,85);

	for(x=0; x<size; x++) {
		tmp.Format("%i",x+1);
		ScoreList.InsertItem(x,tmp,0);
		ScoreList.SetItemText(x,1,OLE2T(entries[x].username));
		tmp.Format("%i",entries[x].combo);
		ScoreList.SetItemText(x,2,tmp);
		tmp.Format("%.2f",entries[x].time);
		ScoreList.SetItemText(x,3,tmp);
		ScoreList.SetItemText(x,4,OLE2T(entries[x].score));
	}

	theApp.statusDlg.ShowWindow(SW_HIDE);
	return true;
}
