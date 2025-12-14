// LukaStosic19424View.h : interface of the CLukaStosic19424View class
//

#pragma once

class CLukaStosic19424Doc;

class CLukaStosic19424View : public CView
{
protected: // create from serialization only
	CLukaStosic19424View() noexcept;
	DECLARE_DYNCREATE(CLukaStosic19424View)

	// Attributes
public:
	CLukaStosic19424Doc* GetDocument() const;
	bool showGrid = false;
	int angle1 = 0;
	int angle2 = 0;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void DrawFigure(CDC* pDC);
	void DrawPot(CDC* pDC);
	void CactusPart(CDC* pDC, HENHMETAFILE* texture, int x, int y, int width, int hight, int angleCenterRot, int angleSelfRot, int angle = 0);
	void RootPart(CDC* pDC, int x, int y, int angleCenterRot);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CLukaStosic19424View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG
inline CLukaStosic19424Doc* CLukaStosic19424View::GetDocument() const
{
	return reinterpret_cast<CLukaStosic19424Doc*>(m_pDocument);
}
#endif
