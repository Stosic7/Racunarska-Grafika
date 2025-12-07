
// Lab1LSView.cpp : implementation of the CLab1LSView class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1LS.h"
#endif

#include "Lab1LSDoc.h"
#include "Lab1LSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab1LSView

IMPLEMENT_DYNCREATE(CLab1LSView, CView)

BEGIN_MESSAGE_MAP(CLab1LSView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLab1LSView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLab1LSView construction/destruction

CLab1LSView::CLab1LSView() noexcept
{
	// TODO: add construction code here

}

CLab1LSView::~CLab1LSView()
{
}

BOOL CLab1LSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CLab1LSView::DrawRotatedPolygon(CDC* pDC, int cx, int cy, int radius, int numSides, double angleDegrees, COLORREF borderColor, int borderWidth) {
	POINT* pts = new POINT[numSides];

	double angleRad = angleDegrees * M_PI / 180.0;

	for (int i = 0; i < numSides; i++) {
		double angle = 2.0 * M_PI * i / numSides - M_PI / 2 + angleRad;
		pts[i].x = cx + (int)(radius * cos(angle));
		pts[i].y = cy + (int)(radius * sin(angle));
	}

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* pOldPen = pDC->SelectObject(pen);

	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Polygon(pts, numSides);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pen;
	delete[] pts;
}


void CLab1LSView::DrawTriangle(CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	POINT pts[3] = { CPoint(x1, y1), CPoint(x2, y2), CPoint(x3, y3) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* pOldPen = pDC->SelectObject(pen);

	CBrush brush(fillColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(pts, 3);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pen;
}

void CLab1LSView::DrawRectangle(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = borderColor;

	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &lb);
	CPen* pOldPen = pDC->SelectObject(pen);

	CBrush brush(fillColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x1, y1, x2, y2);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pen;
}

void CLab1LSView::DrawSquare(CDC* pDC, int x, int y, int size, COLORREF borderColor, int borderWidth, COLORREF fillColor) {
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0, 255, 255);
	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, borderWidth, &logBrush);
	CPen* pOldPen = pDC->SelectObject(pen);

	CBrush brush(HS_CROSS, RGB(200, 220, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x, y, x + size, y + size);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pen;
}

void CLab1LSView::FillBackground(CDC* pDC, COLORREF backColor) {
	CRect rcClient;
	GetClientRect(&rcClient);

	CBrush brush(backColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(rcClient);

	pDC->SelectObject(pOldBrush);
}

void CLab1LSView::DrawGrid(CDC* pDC, int cellSize, COLORREF color) {
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

void CLab1LSView::OnDraw(CDC* pDC)
{
	CLab1LSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	FillBackground(pDC, RGB(180, 180, 180));


	DrawSquare(pDC, 20, 20, 240, RGB(0, 255, 255), 3, RGB(0, 255, 255));
	DrawTriangle(pDC, 20, 20, 140, 140, 20, 260, RGB(0, 255, 255), 3, RGB(0, 200, 0));
	DrawTriangle(pDC, 20, 20, 140, 140, 140, 20, RGB(0, 255, 255), 3, RGB(150, 0, 150));
	DrawRectangle(pDC, 140, 20, 260, 140, RGB(0, 255, 255), 3, RGB(255, 0, 0));
	DrawTriangle(pDC, 260, 140, 260, 260, 140, 260, RGB(0, 255, 255), 3, RGB(255, 165, 0));


	DrawSquare(pDC, 260, 260, 220, RGB(0, 255, 255), 3, RGB(0, 255, 255));
	DrawTriangle(pDC, 260, 260, 480, 480, 480, 260, RGB(0, 255, 255), 3, RGB(255, 255, 0));
	DrawTriangle(pDC, 260, 260, 480, 480, 260, 480, RGB(0, 255, 255), 3, RGB(255, 105, 180));

	DrawRotatedPolygon(pDC, 75, 140, 30, 6, 60, RGB(0, 255, 255), 3);
	DrawRotatedPolygon(pDC, 105, 60, 20, 4, 90, RGB(0, 255, 255), 3);
	DrawRotatedPolygon(pDC, 225, 220, 20, 6, 30, RGB(0, 255, 255), 3);
	DrawRotatedPolygon(pDC, 420, 320, 35, 6, 60, RGB(0, 255, 255), 3);
	DrawRotatedPolygon(pDC, 320, 420, 35, 5, 90, RGB(0, 255, 255), 3);

	DrawGrid(pDC, 20, RGB(240, 240, 240));

}





// CLab1LSView printing


void CLab1LSView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLab1LSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1LSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1LSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLab1LSView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLab1LSView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLab1LSView diagnostics

#ifdef _DEBUG
void CLab1LSView::AssertValid() const
{
	CView::AssertValid();
}

void CLab1LSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1LSDoc* CLab1LSView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1LSDoc)));
	return (CLab1LSDoc*)m_pDocument;
}
#endif //_DEBUG


// CLab1LSView message handlers
