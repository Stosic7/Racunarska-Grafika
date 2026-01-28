
// I KOLOKVIJUM 2022View.h : interface of the CIKOLOKVIJUM2022View class
//

#pragma once
#include "DImage.h"

class CIKOLOKVIJUM2022View : public CView
{
protected: // create from serialization only
	CIKOLOKVIJUM2022View() noexcept;
	DECLARE_DYNCREATE(CIKOLOKVIJUM2022View)

// Attributes
public:
	CIKOLOKVIJUM2022Doc* GetDocument() const;
	DImage* base;
	DImage* arm1;
	DImage* arm2;
	DImage* head;
	DImage* pozadina;
	DImage* base_shadow;
	DImage* arm1_shadow;
	DImage* arm2_shadow;
	DImage* head_shadow;

	float arm1Angle = 0.0;
	float arm2Angle = 0.0;
	float headAngle = 0.0;

// Operations
public:

// Overrides
public:
	void DrawLampShadow(CDC* pDC);
	void DrawLamp(CDC* pDC, bool bIsShadow);
	void DrawLampHead(CDC* pDC, bool bIsShadow);
	void DrawLampArm2(CDC* pDC, bool bIsShadow);
	void DrawLampArm1(CDC* pDC, bool bIsShadow);
	void DrawLampBase(CDC* pDC, bool bIsShadow);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	void Scale(CDC* pDC, float dX, float dY, bool rightMultiply);
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
	virtual ~CIKOLOKVIJUM2022View();
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

#ifndef _DEBUG  // debug version in I KOLOKVIJUM 2022View.cpp
inline CIKOLOKVIJUM2022Doc* CIKOLOKVIJUM2022View::GetDocument() const
   { return reinterpret_cast<CIKOLOKVIJUM2022Doc*>(m_pDocument); }
#endif

