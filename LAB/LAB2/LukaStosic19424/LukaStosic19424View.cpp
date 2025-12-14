// LukaStosic19424View.cpp : implementation of the CLukaStosic19424View class
//

#include "pch.h"
#include "framework.h"

#ifndef SHARED_HANDLERS
#include "LukaStosic19424.h"
#endif

#include "LukaStosic19424Doc.h"
#include "LukaStosic19424View.h"

#include <cmath>

#define TO_RAD(x) ((x) * 3.14159f / 180.0f)

// CLukaStosic19424View

IMPLEMENT_DYNCREATE(CLukaStosic19424View, CView)

BEGIN_MESSAGE_MAP(CLukaStosic19424View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLukaStosic19424View construction/destruction

CLukaStosic19424View::CLukaStosic19424View() noexcept
{
	showGrid = false;
	angle1 = 0;
	angle2 = 0;
}

CLukaStosic19424View::~CLukaStosic19424View()
{
}

// crtanje kaktusa
void CLukaStosic19424View::DrawFigure(CDC* pDC)
{
	HENHMETAFILE lightPart = GetEnhMetaFile(_T("cactus_part_light.emf"));
	HENHMETAFILE darkPart = GetEnhMetaFile(_T("cactus_part.emf"));

	if (lightPart == NULL || darkPart == NULL) AfxMessageBox(_T("Greska pri ucitavanju emf datoteka."));

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);

	CactusPart(pDC, &lightPart, 250, 389, 60, 75, angle1, 0);
	CactusPart(pDC, &lightPart, 304, 262, 40, 75, angle1, 0, angle2);
	CactusPart(pDC, &darkPart, 198, 183, 60, 75, angle1, 0);
	CactusPart(pDC, &darkPart, 198, 259, 40, 75, angle1, 0);
	CactusPart(pDC, &darkPart, 275, 325, 20, 75, angle1, 45);
	CactusPart(pDC, &darkPart, 250, 312, 20, 75, angle1, 0);
	CactusPart(pDC, &darkPart, 225, 325, 20, 75, angle1, -45);
	CactusPart(pDC, &darkPart, 163, 300, 40, 75, angle1, -90);
	CactusPart(pDC, &darkPart, 344, 300, 40, 75, angle1, 90);
	CactusPart(pDC, &darkPart, 408, 271, 40, 75, angle1, 45);
	CactusPart(pDC, &darkPart, 408, 324, 40, 75, angle1, 135);

	RootPart(pDC, 250, 350, angle1);
	RootPart(pDC, 200, 300, angle1);
	RootPart(pDC, 199, 223, angle1);
	RootPart(pDC, 305, 300, angle1);
	RootPart(pDC, 380, 300, angle1);
	RootPart(pDC, 250, 425, angle1);

	pDC->SetGraphicsMode(prevMode);
}

void CLukaStosic19424View::DrawPot(CDC* pDC)
{
	POINT points[8] = {
		CPoint(191, 432),
		CPoint(312, 432),
		CPoint(312, 450),
		CPoint(302, 450),
		CPoint(292, 499),
		CPoint(211, 499),
		CPoint(201, 450),
		CPoint(191, 450)
	};

	CBrush* colorPot = new CBrush(RGB(222, 148, 0));
	CPen* penPot = new CPen(PS_SOLID, 1, RGB(144, 125, 58));

	CBrush* oldBrush = pDC->SelectObject(colorPot);
	CPen* oldPen = pDC->SelectObject(penPot);

	pDC->Polygon(points, 8);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	colorPot->DeleteObject();
	penPot->DeleteObject();
}

void CLukaStosic19424View::CactusPart(CDC* pDC, HENHMETAFILE* texture, int x, int y, int width, int height, int angleCenterRot, int angleSelfRot, int angle)
{
	XFORM xFormOld;
	pDC->GetWorldTransform(&xFormOld);

	width >>= 1;
	height >>= 1;

	Rotate(pDC, TO_RAD(angleSelfRot), true);

	if (angle != 0) {
		Translate(pDC, 0, -height, true);
		Rotate(pDC, TO_RAD(angle), true);
		Translate(pDC, 0, height, true);
	}

	Translate(pDC, x - 250, y - 425, true);
	Rotate(pDC, TO_RAD(angleCenterRot), true);
	Translate(pDC, 250, 425, true);

	pDC->PlayMetaFile(*texture, CRect(-width, -height, width, height));
	pDC->SetWorldTransform(&xFormOld);
}

void CLukaStosic19424View::RootPart(CDC* pDC, int x, int y, int angleCenterRot)
{
	XFORM xFormOld;
	pDC->GetWorldTransform(&xFormOld);

	CBrush brush(RGB(0, 204, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	Translate(pDC, x - 250, y - 425, true);
	Rotate(pDC, TO_RAD(angleCenterRot), true);
	Translate(pDC, 250, 425, true);

	pDC->Ellipse(-10, -10, 10, 10);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	pDC->SetWorldTransform(&xFormOld);
}

BOOL CLukaStosic19424View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CLukaStosic19424View drawing

void CLukaStosic19424View::OnDraw(CDC* pDC)
{
	CLukaStosic19424Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBrush* backgroundBrush = new CBrush(RGB(135, 206, 235));
	CPen* gridPen = new CPen(PS_SOLID, 1, RGB(210, 237, 247));
	CPen* startPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));

	CBrush* oldBrush = pDC->SelectObject(backgroundBrush);
	CPen* oldPen = pDC->SelectObject(startPen);

	pDC->Rectangle(0, 0, 500, 500);

	DrawFigure(pDC);
	DrawPot(pDC);

	if (showGrid) {
		pDC->SelectObject(gridPen);

		for (int i = 25; i < 500; i += 25)
		{
			pDC->MoveTo(0, i);
			pDC->LineTo(500, i);

			pDC->MoveTo(i, 0);
			pDC->LineTo(i, 500);
		}
	}

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

	gridPen->DeleteObject();
	startPen->DeleteObject();
	backgroundBrush->DeleteObject();
}

void CLukaStosic19424View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = 1.0f;
	xForm.eM12 = 0.0f;
	xForm.eM21 = 0.0f;
	xForm.eM22 = 1.0f;
	xForm.eDx = dX;
	xForm.eDy = dY;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = sX;
	xForm.eM12 = 0.0f;
	xForm.eM21 = 0.0f;
	xForm.eM22 = sY;
	xForm.eDx = 0.0f;
	xForm.eDy = 0.0f;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = cosf(angle);
	xForm.eM12 = sinf(angle);
	xForm.eM21 = -sinf(angle);
	xForm.eM22 = cosf(angle);
	xForm.eDx = 0.0f;
	xForm.eDy = 0.0f;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

// CLukaStosic19424View printing

BOOL CLukaStosic19424View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CLukaStosic19424View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CLukaStosic19424View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// CLukaStosic19424View diagnostics

#ifdef _DEBUG
void CLukaStosic19424View::AssertValid() const
{
	CView::AssertValid();
}

void CLukaStosic19424View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLukaStosic19424Doc* CLukaStosic19424View::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLukaStosic19424Doc)));
	return (CLukaStosic19424Doc*)m_pDocument;
}
#endif //_DEBUG

void CLukaStosic19424View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
	case VK_SPACE:
		showGrid = !showGrid; break;
	case 'A':
		angle1 -= 6;
		break;
	case 'D':
		angle1 += 6;
		break;
	case VK_LEFT:
		angle2 -= 6;
		break;
	case VK_RIGHT:
		angle2 += 6;
		break;
	default:
		return;
	}

	Invalidate();
}
