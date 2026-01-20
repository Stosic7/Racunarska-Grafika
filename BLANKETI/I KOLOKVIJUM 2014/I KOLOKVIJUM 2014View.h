
// I KOLOKVIJUM 2014View.h : interface of the CIKOLOKVIJUM2014View class
//
#include <atlimage.h> 
#include <wingdi.h>
#pragma once


class CIKOLOKVIJUM2014View : public CView
{
protected: // create from serialization only
	float m_hPos;
	float m_angle;

	CImage m_pTocak;
	HENHMETAFILE m_met = NULL;
	CIKOLOKVIJUM2014View() noexcept;
	DECLARE_DYNCREATE(CIKOLOKVIJUM2014View)

// Attributes
public:
	CIKOLOKVIJUM2014Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawGround(CDC* pDC, float angle);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIKOLOKVIJUM2014View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in I KOLOKVIJUM 2014View.cpp
inline CIKOLOKVIJUM2014Doc* CIKOLOKVIJUM2014View::GetDocument() const
   { return reinterpret_cast<CIKOLOKVIJUM2014Doc*>(m_pDocument); }
#endif

