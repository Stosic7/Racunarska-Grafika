// I KOLOKVIJUM 2014View.cpp : implementation of the CIKOLOKVIJUM2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I KOLOKVIJUM 2014.h"
#endif

#include "I KOLOKVIJUM 2014Doc.h"
#include "I KOLOKVIJUM 2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

constexpr double toRad = 3.14159265358979323846 / 180.0;
constexpr double toDeg = 180.0 / 3.14159265358979323846;

// CIKOLOKVIJUM2014View

IMPLEMENT_DYNCREATE(CIKOLOKVIJUM2014View, CView)

BEGIN_MESSAGE_MAP(CIKOLOKVIJUM2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIKOLOKVIJUM2014View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIKOLOKVIJUM2014View construction/destruction

CIKOLOKVIJUM2014View::CIKOLOKVIJUM2014View() noexcept
{
	m_hPos = 0.0f;
	m_angle = 10.0f;

	HRESULT hr = m_pTocak.Load(_T("D:\\RG PRIPREMA\\IKOL2024 RESURSI\\Wheel.png"));
	m_met = GetEnhMetaFile(_T("D:\\RG PRIPREMA\\IKOL2024 RESURSI\\clio.emf"));

}

CIKOLOKVIJUM2014View::~CIKOLOKVIJUM2014View()
{
	DeleteEnhMetaFile(m_met);
}

BOOL CIKOLOKVIJUM2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKOLOKVIJUM2014View drawing

void CIKOLOKVIJUM2014View::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	if (m_pTocak.IsNull()) return;

	XFORM transOld;
	pDC->GetWorldTransform(&transOld);

	Translate(pDC, x, y, false);
	Rotate(pDC, angle, false);

	int dx = 52, dy = 15, w = 376;
	CRect srcRc(dx, dy, dx + w, dy + w);
	CRect dstRc(-r, -r, r, r);

	srcRc.NormalizeRect();
	dstRc.NormalizeRect();

	m_pTocak.TransparentBlt(pDC->GetSafeHdc(), dstRc, srcRc, RGB(255, 255, 255));

	pDC->SetWorldTransform(&transOld);
}

void CIKOLOKVIJUM2014View::DrawCar(CDC* pDC, int x, int y, int w, int h)
{
	CRect rc(x + w / 2, y - h / 2, x - w / 2, y + h / 2);
	pDC->PlayMetaFile(m_met, rc);
}

void CIKOLOKVIJUM2014View::Scale(CDC* pDC, float sX, float sY, bool rightMultply)
{
	XFORM trans;
	trans.eM11 = sX;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = sY;
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	if (rightMultply)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2014View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM trans;
	trans.eM11 = cos(angle * toRad);
	trans.eM12 = sin(angle * toRad);
	trans.eM21 = -sin(angle * toRad);
	trans.eM22 = cos(angle * toRad);
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2014View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans;
	trans.eM11 = 1.0;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = 1.0;
	trans.eDx = dX;
	trans.eDy = dY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&trans, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&trans, MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2014View::DrawGround(CDC* pDC, float angle)
{
	CRect rect;
	GetClientRect(&rect);

	CPen pen(PS_SOLID, 3, RGB(128, 32, 32));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush(HS_FDIAGONAL, RGB(128, 32, 32));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	POINT pts[3];
	pts[0].x = rect.left; pts[0].y = rect.bottom;
	pts[1].x = rect.right; pts[1].y = rect.bottom;
	pts[2].x = rect.right; pts[2].y = rect.bottom - rect.Width() * tan(angle * toRad);

	COLORREF oldBkColor = pDC->SetBkColor(RGB(220, 255, 192));
	
	pDC->Polygon(pts, 3);
	pDC->SetBkColor(oldBkColor);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CIKOLOKVIJUM2014View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	CBitmap bmp;

	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC->SelectObject(&bmp);

	CBrush brush(RGB(200, 220, 255));
	CBrush* pOldBrush = pMemDC->SelectObject(&brush);

	pMemDC->Rectangle(0, 0, rect.Width(), rect.Height());
	pMemDC->SelectObject(pOldBrush);

	XFORM transOld;
	int gm = pMemDC->SetGraphicsMode(GM_ADVANCED);
	pMemDC->GetWorldTransform(&transOld);

	float angle = m_angle;
	DrawGround(pMemDC, angle);

	int carWidth = 450;
	int carHeight = carWidth / 2.5;
	int x = 0 + m_hPos, y = 0, r = 38;
	float alpha = toDeg * m_hPos / (2 * 3.14159265358979323846);

	pMemDC->GetWorldTransform(&transOld);

	Translate(pMemDC, carWidth / 2.0, -carHeight / 2.0 - r / 2.0, true);
	Rotate(pMemDC, -angle, true);
	Translate(pMemDC, 0.0, rect.Height(), true);

	DrawCar(pMemDC, x, y, carWidth, carWidth / 2.5);

	DrawWheel(pMemDC, x - 155, y + 70, r, alpha);
	DrawWheel(pMemDC, x + 135, y + 70, r, alpha);

	pMemDC->SetWorldTransform(&transOld);
	pMemDC->SetGraphicsMode(gm);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
}


// CIKOLOKVIJUM2014View printing


void CIKOLOKVIJUM2014View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIKOLOKVIJUM2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKOLOKVIJUM2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKOLOKVIJUM2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIKOLOKVIJUM2014View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIKOLOKVIJUM2014View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CIKOLOKVIJUM2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		m_hPos += 10.0;
		Invalidate();
	}
	if (nChar == VK_LEFT) {
		m_hPos -= 10.0;
		if (m_hPos < 0) {
			m_hPos = 0.0;
		}
		Invalidate();
	}
	if (nChar == VK_UP) {
		m_hPos = 0.0;
		m_angle += 10.0f;
		if (m_angle > 80.0f) {
			m_angle = 80.0f;
		}
		Invalidate();
	}
	if (nChar == VK_DOWN) {
		m_hPos = 0.0f;
		m_angle -= 10.0f;
		if (m_angle < -10.0) {
			m_angle = -10.0f;
		}
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CIKOLOKVIJUM2014View::OnEraseBkgnd(CDC* pDC)
{
	return true;
}



// CIKOLOKVIJUM2014View diagnostics

#ifdef _DEBUG
void CIKOLOKVIJUM2014View::AssertValid() const
{
	CView::AssertValid();
}

void CIKOLOKVIJUM2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKOLOKVIJUM2014Doc* CIKOLOKVIJUM2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKOLOKVIJUM2014Doc)));
	return (CIKOLOKVIJUM2014Doc*)m_pDocument;
}
#endif //_DEBUG


