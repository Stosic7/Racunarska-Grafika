#pragma once
#include "DImage.h"

class CLukaStosic19424View : public CView
{
protected:
	CLukaStosic19424View() noexcept;
	DECLARE_DYNCREATE(CLukaStosic19424View)

public:
	CLukaStosic19424Doc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	void DrawGrid(CDC* memDC);
	void MakeItGray(DImage* img);
	void MakeItBlue(DImage* img);
	void MemDCTopDC(CDC* memDC, CDC* pDC);
	void MakeItRed(DImage* img);
	void DrawTransparent(CDC* pDC, DImage* img, int x, int y);
	void SetPuzzle(CDC* pDC, CBitmap* bmp);

	DImage** part;
	const float C_PI = 3.141592653589793;
	const float stepAngle = C_PI / 180;

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CLukaStosic19424View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG
inline CLukaStosic19424Doc* CLukaStosic19424View::GetDocument() const
{
	return reinterpret_cast<CLukaStosic19424Doc*>(m_pDocument);
}
#endif
