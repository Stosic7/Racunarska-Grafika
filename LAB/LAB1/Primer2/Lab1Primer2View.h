
// Lab1Primer2View.h : interface of the CLab1Primer2View class
//

#pragma once


class CLab1Primer2View : public CView
{
protected: // create from serialization only
	CLab1Primer2View() noexcept;
	DECLARE_DYNCREATE(CLab1Primer2View)

// Attributes
public:
	CLab1Primer2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void drawRectangles(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF boredColor, int borderWidth, COLORREF fillColor);
	void drawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void drawPrimarySquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void fillBackground(CDC* pDC, COLORREF backColor);
	void drawGrid(CDC* pDC, int cellSize, COLORREF color);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab1Primer2View();
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

#ifndef _DEBUG  // debug version in Lab1Primer2View.cpp
inline CLab1Primer2Doc* CLab1Primer2View::GetDocument() const
   { return reinterpret_cast<CLab1Primer2Doc*>(m_pDocument); }
#endif

