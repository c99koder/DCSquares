// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DCSquares-MFC.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	RECT r;

	cs.style = WS_OVERLAPPED | WS_CAPTION
		 | WS_MINIMIZEBOX | WS_SYSMENU;
	r.top=0;
	r.left=0;
	r.right=640;
	r.bottom=480;
	AdjustWindowRect(&r,cs.style,1);
	cs.cx=r.right-r.left;
	cs.cy=r.bottom-r.top;
	cs.x=(::GetSystemMetrics(SM_CXSCREEN) - cs.cx)/2;
	cs.y=(::GetSystemMetrics(SM_CYSCREEN) - cs.cy)/2;
	cs.lpszName="DCSquares";
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG