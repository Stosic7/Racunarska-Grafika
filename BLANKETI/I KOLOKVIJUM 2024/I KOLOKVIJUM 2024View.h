
// I KOLOKVIJUM 2024View.h : interface of the CIKOLOKVIJUM2024View class
//

#pragma once
#include "DImage.h"

class CIKOLOKVIJUM2024View : public CView
{
protected: // create from serialization only
	CIKOLOKVIJUM2024View() noexcept;
	DECLARE_DYNCREATE(CIKOLOKVIJUM2024View)

// Attributes
public:
	CIKOLOKVIJUM2024Doc* GetDocument() const;
	DImage* arm;
	DImage* background;
	DImage* basket;
	double platformAngle = 0.0f;
	double offset = 0.0;
	double alpha = -90.0;
	double beta = -90.0;

// Operations
public:

// Overrides
public:
	void DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle);
	void DrawPlatform(CDC* pDC, int l, int r, double angle);
	void DrawBasketCouple(CDC* pDC, int l, int r, float angle);
	void DrawBasket(CDC* pDC, int r);
	void DrawArm(CDC* pDC);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawImageTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIKOLOKVIJUM2024View();
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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in I KOLOKVIJUM 2024View.cpp
inline CIKOLOKVIJUM2024Doc* CIKOLOKVIJUM2024View::GetDocument() const
   { return reinterpret_cast<CIKOLOKVIJUM2024Doc*>(m_pDocument); }
#endif

