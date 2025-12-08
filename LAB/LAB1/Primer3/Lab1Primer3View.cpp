
// Lab1Primer3View.cpp : implementation of the CLab1Primer3View class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1Primer3.h"
#endif

#include "Lab1Primer3Doc.h"
#include "Lab1Primer3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab1Primer3View

IMPLEMENT_DYNCREATE(CLab1Primer3View, CView)

BEGIN_MESSAGE_MAP(CLab1Primer3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLab1Primer3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLab1Primer3View construction/destruction

CLab1Primer3View::CLab1Primer3View() noexcept
{
	// TODO: add construction code here

}

CLab1Primer3View::~CLab1Primer3View()
{
}

BOOL CLab1Primer3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab1Primer3View drawing

void CLab1Primer3View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {

	POINT* pts = new POINT[n];

	for (int i = 0; i < n; i++) {
		float centralAngle = 2.0f * M_PI * i;
		float angle = (centralAngle / n) + rotAngle;

		int x = cx + static_cast<int>(r * cos(angle));
		int y = cy + static_cast<int>(r * sin(angle));

		pts[i] = CPoint(x, y);
	}

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0, 206, 209);

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 4, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	pDC->Polygon(pts, n);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);

	delete[] pts;
}

void CLab1Primer3View::drawRectangle(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(fillColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x1, y1, x2, y2);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}

void CLab1Primer3View::DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int boredWidth, COLORREF fillColor) {
	POINT pts[3] = { CPoint(x1, y1), CPoint(x2, y2), CPoint(x3, y3) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, boredWidth, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(fillColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(pts, 3);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}

void CLab1Primer3View::FillBackground(CDC* pDC, COLORREF backColor) {
	CBrush brush(backColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(&oldBrush);
}

void CLab1Primer3View::DrawGrid(CDC* pDC, int cellSize, COLORREF color) {
	const int width = 500;
	const int height = 500;

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = color;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 1, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	for (int x = 0; x < width; x += cellSize) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, height);
	}

	for (int y = 0; y < height; y += cellSize) {
		pDC->MoveTo(0, y);
		pDC->LineTo(width, y);
	}

	pDC->SelectObject(&oldPen);
}

void CLab1Primer3View::OnDraw(CDC* pDC)
{
	CLab1Primer3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	FillBackground(pDC, RGB(180, 180, 180));

	//orange triangle
	DrawTriangle(pDC, 25, 10 * 25, 5 * 25, 6 * 25, 5 * 25, 14 * 25, RGB(0, 206, 209), 5, RGB(255, 165, 0));

	//green square
	drawRectangle(pDC, 5 * 25, 8 * 25 + 19, 7 * 25 + 10, 11 * 25 + 10, RGB(0, 206, 209), 5, RGB(0, 255, 0));
	
	//red triangle
	DrawTriangle(pDC,
		7 * 25 + 10, 8 * 25 + 19, 
		7 * 25 + 10, 11 * 25 + 10, 
		10 * 25, 11 * 25 + 10, 
		RGB(0, 206, 209), 5, RGB(255, 0, 0));

	// H-Cross triangle
	POINT pts[3] = { CPoint(7*25+10, 8*25+19), CPoint(10*25, 11*25+10), CPoint(13*25, 8*25+19) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0,206, 209);

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 5, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(HS_CROSS, RGB(0,0,255));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(pts, 3);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);

	// Pink polygone
	POINT pts2[4] = { CPoint(13*25, 8*25+19), CPoint(10*25, 11*25+10), CPoint(13*25, 11*25+10), CPoint(16*25, 8*25+19)};

	LOGBRUSH lb2;
	lb2.lbStyle = BS_SOLID;
	lb2.lbColor = RGB(0, 206, 209);

	CPen pen2(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 5, &lb2);
	CPen* oldPen2 = pDC->SelectObject(&pen2);

	CBrush brush2(RGB(255, 192, 203));
	CBrush* oldBrush2 = pDC->SelectObject(&brush2);

	pDC->Polygon(pts2, 4);

	pDC->SelectObject(&oldPen2);
	pDC->SelectObject(&oldBrush2);

	// purple triangle
	DrawTriangle(pDC, 16 * 25, 8 * 25 + 19, 13 * 25, 11 * 25 + 10, 16 * 25, 11 * 25 + 10, RGB(0, 206, 209), 5, RGB(128, 0, 128));

	//yellow Triangle
	DrawTriangle(pDC, 16 * 25, 6 * 25, 16 * 25, 14 * 25, 19 * 25, 10 * 25, RGB(0, 206, 209), 5, RGB(255, 255, 0));

	//polygons

	DrawRegularPolygon(pDC, 85, 250, 20, 8, 0);
	DrawRegularPolygon(pDC, 205, 267, 10, 5, 0);
	DrawRegularPolygon(pDC, 254, 241, 15, 4, 0);
	DrawRegularPolygon(pDC, 380, 263, 10, 7, 0);
	DrawRegularPolygon(pDC, 430, 250, 20, 5, 0);

	DrawGrid(pDC, 25, RGB(240, 240, 240));
}


// CLab1Primer3View printing


void CLab1Primer3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLab1Primer3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1Primer3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1Primer3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLab1Primer3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLab1Primer3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLab1Primer3View diagnostics

#ifdef _DEBUG
void CLab1Primer3View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1Primer3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Primer3Doc* CLab1Primer3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Primer3Doc)));
	return (CLab1Primer3Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab1Primer3View message handlers
