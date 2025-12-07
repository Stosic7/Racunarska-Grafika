
// Lab1Primer2View.cpp : implementation of the CLab1Primer2View class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1Primer2.h"
#endif

#include "Lab1Primer2Doc.h"
#include "Lab1Primer2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab1Primer2View

IMPLEMENT_DYNCREATE(CLab1Primer2View, CView)

BEGIN_MESSAGE_MAP(CLab1Primer2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLab1Primer2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLab1Primer2View construction/destruction

CLab1Primer2View::CLab1Primer2View() noexcept
{
	// TODO: add construction code here

}

CLab1Primer2View::~CLab1Primer2View()
{
}

BOOL CLab1Primer2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab1Primer2View drawing



void CLab1Primer2View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	CPoint* points = new CPoint[n];

	for (int i = 0; i < n; i++) {
		float centralAngle = 2.0f * M_PI * i;
		float angle = (centralAngle / n) + rotAngle;

		int x = cx + static_cast<int>(r * cos(angle));
		int y = cy + static_cast<int>(r * sin(angle));

		points[i] = CPoint(x, y);
	}

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0,0,255);

	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 4, &lb);
	CPen* pOldPen = pDC->SelectObject(pen);

	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	pDC->Polygon(points, n);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pen;
	delete[] points;
}

void CLab1Primer2View::drawRectangles(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF boredColor, int borderWidth, COLORREF fillColor) {
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = boredColor;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(fillColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x1, y1, x2, y2);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}

void CLab1Primer2View::drawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	POINT pts[3] = { CPoint(x1, y1), CPoint(x2, y2), CPoint(x3, y3) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(fillColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(pts, 3);

	pDC->SelectObject(&oldBrush);
	pDC->SelectObject(&oldPen);
}

void CLab1Primer2View::drawPrimarySquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	if (size < 0) return;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = borderColor;

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &logBrush);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(RGB(255, 255, 255));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x, y, x + size, y + size);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);
}

void CLab1Primer2View::fillBackground(CDC* pDC, COLORREF backColor) {
	CBrush brush(backColor);
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0,0,500,500);

	pDC->SelectObject(&oldBrush);
}

void CLab1Primer2View::drawGrid(CDC* pDC, int cellSize, COLORREF color) {
	const int width = 500;
	const int height = 500;

	CPen pen(PS_SOLID, 1, color);
	CPen* oldPen = pDC->SelectObject(&pen);

	for (int x = 0; x <= width; x += cellSize) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, height);
	}

	for (int y = 0; y <= height; y += cellSize) {
		pDC->MoveTo(0, y);
		pDC->LineTo(width, y);
	}

	pDC->SelectObject(oldPen);
}

void CLab1Primer2View::OnDraw(CDC* pDC)
{
	CLab1Primer2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	fillBackground(pDC, RGB(180, 180, 180));
	drawGrid(pDC, 25, RGB(240, 240, 240));

	// main square
	drawPrimarySquare(pDC, 25, 25, 450, RGB(0, 0, 255), 5, RGB(0, 0, 255));
	
	//triangles
	drawTriangle(pDC, 25, 25, 175, 175, 25, 175, RGB(0, 0, 255), 5, RGB(255, 0, 0));
	drawTriangle(pDC, 25, 25, 13*25, 13*25, 13*25, 25, RGB(0, 0, 255), 5, RGB(128, 0, 128));
	drawTriangle(pDC, 7 * 25, 7 * 25, 7 * 25, 19 * 25, 13 * 25, 13 * 25, RGB(0, 0, 255), 5, RGB(0, 255, 0));
	drawTriangle(pDC, 13 * 25, 13 * 25, 13 * 25, 7 * 25, 19 * 25, 7 * 25, RGB(0, 0, 255), 5, RGB(160, 160, 160));
	drawTriangle(pDC, 25, 19 * 25, 25, 14 * 25, 7 * 25, 19 * 25, RGB(0, 0, 255), 5, RGB(255, 165, 0));
	drawTriangle(pDC, 25, 175, 175, 175, 175, 325, RGB(0, 0, 255), 5, RGB(235, 235, 235));

	//One polygon, pink
	POINT pts[4] = { CPoint(25, 7 * 25), CPoint(7 * 25, 13 * 25), CPoint(7 * 25, 19 * 25), CPoint(25, 14 * 25) };

	CBrush pinkBrush(RGB(255, 150, 200));
	CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);
	CPen pen(PS_SOLID, 5, RGB(0, 0, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->Polygon(pts, 4);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	// one square
	drawRectangles(pDC, 13 * 25, 25, 19 * 25, 7 * 25, RGB(0, 0, 255), 5, RGB(255, 255, 0));

	// draw HS_CROSS square triangle
	POINT pts2[3] = { CPoint(7 * 25, 19 * 25), CPoint(19 * 25, 19 * 25), CPoint(19 * 25, 7 * 25) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0,0,255);

	CPen pen2(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 5, &lb);
	CPen* oldPen2 = pDC->SelectObject(&pen2);

	CBrush brush2(HS_CROSS, RGB(0, 0, 235));
	CBrush* oldBrush2 = pDC->SelectObject(&brush2);

	pDC->Polygon(pts2, 3);

	pDC->SelectObject(&oldPen2);
	pDC->SelectObject(&oldBrush2);

	// polygons
	DrawRegularPolygon(pDC, 70, 135, 25, 8, 0);
	DrawRegularPolygon(pDC, 240, 113, 45, 6, 0);
	DrawRegularPolygon(pDC, 70, 435, 25, 4, 0);
	DrawRegularPolygon(pDC, 235, 325, 35, 5, 0);
	DrawRegularPolygon(pDC, 390, 380, 50, 7, 0);
}


// CLab1Primer2View printing


void CLab1Primer2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLab1Primer2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1Primer2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1Primer2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLab1Primer2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLab1Primer2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLab1Primer2View diagnostics

#ifdef _DEBUG
void CLab1Primer2View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1Primer2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Primer2Doc* CLab1Primer2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Primer2Doc)));
	return (CLab1Primer2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab1Primer2View message handlers
