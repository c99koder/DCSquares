// DCSquares-MFCView.h : interface of the CDCSquaresMFCView class
//

#include "squares.h"
#pragma once


class CDCSquaresMFCView : public CView
{
protected: // create from serialization only
	CDCSquaresMFCView();
	DECLARE_DYNCREATE(CDCSquaresMFCView)

// Attributes
public:
	CDCSquaresMFCDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CDCSquaresMFCView();
	HGLRC m_hglrc;
	HDC m_hgldc;

	BOOL SetPixelformat(HDC hdc);
	void ReSizeGLScene(int width, int height);
	void GetGLInfo();
	int InitGL(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);

	// Game state
	int state;
	squarelist *player;
	int mousex;
	int mousey;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnHelpHowtoplay();
};

#ifndef _DEBUG  // debug version in DCSquares-MFCView.cpp
inline CDCSquaresMFCDoc* CDCSquaresMFCView::GetDocument() const
   { return reinterpret_cast<CDCSquaresMFCDoc*>(m_pDocument); }
#endif

