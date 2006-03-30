// DCSquares-MFCDoc.h : interface of the CDCSquaresMFCDoc class
//


#pragma once

class CDCSquaresMFCDoc : public CDocument
{
protected: // create from serialization only
	CDCSquaresMFCDoc();
	DECLARE_DYNCREATE(CDCSquaresMFCDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDCSquaresMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


