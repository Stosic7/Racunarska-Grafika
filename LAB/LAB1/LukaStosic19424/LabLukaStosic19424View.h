#pragma once


class CLabLukaStosic19424View : public CView
{
protected: // create from serialization only
	CLabLukaStosic19424View() noexcept;
	DECLARE_DYNCREATE(CLabLukaStosic19424View)

// Attributes
public:
	CLabLukaStosic19424Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void DrawYellowTriangle(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawPinkTriangle(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawOrangeTriangle(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawRedSquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawPurpleTriange(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawBigGreenTriangle(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
	void DrawSquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor);
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
	virtual ~CLabLukaStosic19424View();
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

#ifndef _DEBUG  // debug version in LabLukaStosic19424View.cpp
inline CLabLukaStosic19424Doc* CLabLukaStosic19424View::GetDocument() const
   { return reinterpret_cast<CLabLukaStosic19424Doc*>(m_pDocument); }
#endif

