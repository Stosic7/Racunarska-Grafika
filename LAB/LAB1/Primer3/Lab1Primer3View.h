
// Lab1Primer3View.h : interface of the CLab1Primer3View class
//

#pragma once


class CLab1Primer3View : public CView
{
protected: // create from serialization only
	CLab1Primer3View() noexcept;
	DECLARE_DYNCREATE(CLab1Primer3View)

// Attributes
public:
	CLab1Primer3Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void drawRectangle(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int boredWidth, COLORREF fillColor);
	void FillBackground(CDC* pDC, COLORREF backColor);
	void DrawGrid(CDC* pDC, int cellSize, COLORREF color);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab1Primer3View();
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
};

#ifndef _DEBUG  // debug version in Lab1Primer3View.cpp
inline CLab1Primer3Doc* CLab1Primer3View::GetDocument() const
   { return reinterpret_cast<CLab1Primer3Doc*>(m_pDocument); }
#endif

