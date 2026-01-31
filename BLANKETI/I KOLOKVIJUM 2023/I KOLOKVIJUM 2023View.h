
// IKOLOKVIJUM2023View.h : interface of the CIKOLOKVIJUM2023View class
//

#pragma once
#include "DImage.h"

class CIKOLOKVIJUM2023View : public CView
{
protected: // create from serialization only
	CIKOLOKVIJUM2023View() noexcept;
	DECLARE_DYNCREATE(CIKOLOKVIJUM2023View)

	DImage* glava;
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* podkolenica;
	DImage* podlaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;

	float nadlakticaAngle = 0.0;
	float podlakticaAngle = 0.0;
	float sakaAngle = 0.0;
	float ceo = 0.0;
	float robotScale = 1.0;

// Attributes
public:
	CIKOLOKVIJUM2023Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawRobot(CDC* pDC);
	void DrawHead(CDC* pDC);
	void DrawHalf(CDC* pDC);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIKOLOKVIJUM2023View();
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

#ifndef _DEBUG  // debug version in IKOLOKVIJUM2023View.cpp
inline CIKOLOKVIJUM2023Doc* CIKOLOKVIJUM2023View::GetDocument() const
   { return reinterpret_cast<CIKOLOKVIJUM2023Doc*>(m_pDocument); }
#endif

