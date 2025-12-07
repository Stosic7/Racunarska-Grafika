
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

void CLab1LSView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	CPoint* pts = new CPoint[n];

	for (int i = 0; i < n; i++) {
		float centralAngle = 2.0f * M_PI * i;
		float angle = (centralAngle / n) + rotAngle;

		int x = cx + static_cast<int>(r * cos(angle));
		int y = cy + static_cast<int>(r * sin(angle));

		pts[i] = CPoint(x, y);
	}

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0, 0, 255);

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 4, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	pDC->Polygon(pts, n);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);

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

void CLab1LSView::FillBackground(CDC* pDC, COLORREF backColor) {

	CBrush brush(backColor);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(0,0,500,500);

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
	DrawGrid(pDC, 25, RGB(240, 240, 240));

	// square 1

	DrawRectangle(pDC, 25, 25, 10 * 25, 10 * 25, RGB(0, 255, 255), 5, RGB(255, 255, 255));

	DrawTriangle(pDC, 25, 25, 140, 25, 140, 140, RGB(0, 255, 255), 5, RGB(128, 0, 128));
	DrawTriangle(pDC, 25, 25, 25, 250, 140, 140, RGB(0, 255, 255), 5, RGB(0, 255, 0));
	DrawTriangle(pDC, 140, 250, 250, 250, 250, 140, RGB(0, 255, 255), 5, RGB(255, 165, 0));

	DrawRectangle(pDC, 140, 25, 250, 140, RGB(0, 255, 255), 5, RGB(255, 0, 0));

	// one polygon
	CPoint pts[4] = { CPoint(25, 250), CPoint(140, 140), CPoint(250, 140), CPoint(140, 250) };

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0, 255, 255);

	CPen pen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, 5, &lb);
	CPen* oldPen = pDC->SelectObject(&pen);

	CBrush brush(HS_CROSS, RGB(0, 0, 255));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	pDC->Polygon(pts, 4);

	pDC->SelectObject(&oldPen);
	pDC->SelectObject(&oldBrush);

	// square 2

	DrawRectangle(pDC, 250, 250, 475, 475, RGB(0, 255, 255), 5, RGB(255, 255, 255));

	DrawTriangle(pDC, 250, 250, 475, 475, 475, 250, RGB(0,255,255), 5, RGB(255, 255, 0));
	DrawTriangle(pDC, 250, 250, 475, 475, 250, 475, RGB(0, 255, 255), 5, RGB(255, 192, 203));


	// polygons
	DrawRegularPolygon(pDC, 73, 140, 25, 6, 79);
	DrawRegularPolygon(pDC, 110, 55, 20, 4, 0);
	DrawRegularPolygon(pDC, 215, 215, 17, 6, 0);
	DrawRegularPolygon(pDC, 400, 315, 35, 8, 0);
	DrawRegularPolygon(pDC, 315, 410, 35, 5, 0);
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
