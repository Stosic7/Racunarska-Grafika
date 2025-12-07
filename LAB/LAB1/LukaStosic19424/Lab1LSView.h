
// Lab1LSView.h : interface of the CLab1LSView class
//

#pragma once


class CLab1LSView : public CView
{
protected: // create from serialization only
	CLab1LSView() noexcept;
	DECLARE_DYNCREATE(CLab1LSView)

// Attributes
public:
	CLab1LSDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawRotatedPolygon(CDC* pDC, int cx, int cy, int radius, int numSides, double angleDegrees, COLORREF borderColor, int borderWidth);
	void DrawSquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawRectangle(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void FillBackground(CDC* pDC, COLORREF backColor);
	void DrawGrid(CDC* pDC, int cellSize, COLORREF color);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab1LSView();
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

#ifndef _DEBUG  // debug version in Lab1LSView.cpp
inline CLab1LSDoc* CLab1LSView::GetDocument() const
   { return reinterpret_cast<CLab1LSDoc*>(m_pDocument); }
#endif

