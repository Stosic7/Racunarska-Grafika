
// IKOLOKVIJUM2018View.h : interface of the CIKOLOKVIJUM2018View class
//

#pragma once
#include "DImage.h"


class CIKOLOKVIJUM2018View : public CView
{
protected: // create from serialization only
	CIKOLOKVIJUM2018View() noexcept;
	DECLARE_DYNCREATE(CIKOLOKVIJUM2018View)
	
	DImage* bager;
	DImage* arm1;
	DImage* arm2;
	DImage* pozadina;
	HENHMETAFILE viljuska;

	float arm1Angle = 0.0;
	float arm2Angle = 0.0;
	float forkAngle = 0.0;
	float move = 0.0;

// Attributes
public:
	CIKOLOKVIJUM2018Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawExcavator(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawBody(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIKOLOKVIJUM2018View();
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

#ifndef _DEBUG  // debug version in IKOLOKVIJUM2018View.cpp
inline CIKOLOKVIJUM2018Doc* CIKOLOKVIJUM2018View::GetDocument() const
   { return reinterpret_cast<CIKOLOKVIJUM2018Doc*>(m_pDocument); }
#endif

