// DCSquares-MFCDoc.cpp : implementation of the CDCSquaresMFCDoc class
//

#include "stdafx.h"
#include "DCSquares-MFC.h"

#include "DCSquares-MFCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDCSquaresMFCDoc

IMPLEMENT_DYNCREATE(CDCSquaresMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CDCSquaresMFCDoc, CDocument)
END_MESSAGE_MAP()


// CDCSquaresMFCDoc construction/destruction

CDCSquaresMFCDoc::CDCSquaresMFCDoc()
{
	// TODO: add one-time construction code here

}

CDCSquaresMFCDoc::~CDCSquaresMFCDoc()
{
}

BOOL CDCSquaresMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDCSquaresMFCDoc serialization

void CDCSquaresMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDCSquaresMFCDoc diagnostics

#ifdef _DEBUG
void CDCSquaresMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCSquaresMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDCSquaresMFCDoc commands
