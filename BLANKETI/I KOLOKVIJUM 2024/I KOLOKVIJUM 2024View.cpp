
// I KOLOKVIJUM 2024View.cpp : implementation of the CIKOLOKVIJUM2024View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I KOLOKVIJUM 2024.h"
#endif

#include "I KOLOKVIJUM 2024Doc.h"
#include "I KOLOKVIJUM 2024View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIKOLOKVIJUM2024View

IMPLEMENT_DYNCREATE(CIKOLOKVIJUM2024View, CView)

BEGIN_MESSAGE_MAP(CIKOLOKVIJUM2024View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIKOLOKVIJUM2024View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKOLOKVIJUM2024View construction/destruction

CIKOLOKVIJUM2024View::CIKOLOKVIJUM2024View() noexcept
{
	arm = new DImage();
	background = new DImage();
	basket = new DImage();

	arm->Load(CString("slike/arm.png"));
	background->Load(CString("slike/background.jpg"));
	basket->Load(CString("slike/basket.png"));
}

CIKOLOKVIJUM2024View::~CIKOLOKVIJUM2024View()
{
	delete arm, background, basket;
}

BOOL CIKOLOKVIJUM2024View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKOLOKVIJUM2024View drawing

void CIKOLOKVIJUM2024View::DrawCarousel(CDC* pDC, int h, int r, double offset, double alpha, double beta, double angle)
{
	Translate(pDC, offset, h, false);

	// Rotacija korpe proporcionalna predjenom putu (ako je ovo tacna formula, jebem li ga)
	double rotationAngle = (offset / r) * (180.0 / 3.14159);
	
	// prva korpa, koju pomeramo sa tasterima.
	Rotate(pDC, rotationAngle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -rotationAngle, false);

	// Из ове корпе полази држач центриран у својој осовини који је заротиран за угао alpha у односу на у осу
	Rotate(pDC, alpha, false);
	DrawArm(pDC);

	// Idemo na kraj prvog drzaca i tu crtamo sledeci drzac za ugao beta
	Translate(pDC, 182, 0, false);
	Rotate(pDC, beta, false);
	DrawArm(pDC);

	// pomeramo koordinatni sistem na kraj drugog drzaca i na njega crtamo platformu
	Translate(pDC, 182, 0, false);
	DrawPlatform(pDC, 182, r, angle);

	// vracamo sve transformacije
	Translate(pDC, -182, 0, false);
	Rotate(pDC, -beta, false);
	Translate(pDC, -182, 0, false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -offset, -h, false);
}


void CIKOLOKVIJUM2024View::DrawPlatform(CDC* pDC, int l, int r, double angle)
{
	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -angle, false);

	for (int i = 0; i < 6; i++) {
		Rotate(pDC, 60, false);
		Translate(pDC, 2.0 * r, 0, false);
		DrawBasketCouple(pDC, l, r, -angle);
		Translate(pDC, -2.0 * r, 0, false);
	}
}


void CIKOLOKVIJUM2024View::DrawBasketCouple(CDC* pDC, int l, int r, float angle)
{
	Rotate(pDC, angle, false); // "Она треба да буде заротирана за угао angle око своје осе."
	DrawBasket(pDC, r); // crta se u centru po defaultu, iz funkcije DrawBasket
	Rotate(pDC, -angle, false);

	Translate(pDC, l, 0, false);
	Rotate(pDC, angle, false);
	DrawBasket(pDC, r);
	Rotate(pDC, -angle, false);
	Translate(pDC, -l, 0, false);

	Translate(pDC, 0.8 * r, 0, false); // "тачки помереној водоравно за 0.8*r од центра прве корпе је причвршћен држач његовом већом осовином"
	DrawArm(pDC);
	Translate(pDC, -(0.8 * r), 0, false);
}


void CIKOLOKVIJUM2024View::DrawBasket(CDC* pDC, int r)
{
	DImage* img = basket;
	Scale(pDC, 0.675, 0.675, false);
	Translate(pDC, -(img->Width() / 2), -(img->Height() / 2), false);
	DrawImageTransparent(pDC, img);
	Translate(pDC, (img->Width() / 2), (img->Height() / 2), false);
	Scale(pDC, 1.0 / 0.675, 1.0 / 0.675, false);

	Rotate(pDC, -30, false);
	CFont font;
	font.CreateFont(r * 0.9, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, _T("Verdana"));
	CFont* pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0, 0, 139));
	pDC->SetBkMode(TRANSPARENT);
	CString text = _T("R_G");
	CSize textSize = pDC->GetTextExtent(text);

	pDC->TextOut(-textSize.cx / 2, -textSize.cy / 2, text);
	Rotate(pDC, 30, false);
	pDC->SelectObject(pOldFont);
}

void CIKOLOKVIJUM2024View::DrawArm(CDC* pDC)
{
	DImage* img = arm;
	Rotate(pDC, -90, false);
	Translate(pDC, -10, -10, false);
	DrawImageTransparent(pDC, img);
	Translate(pDC, 10, 10, false);
	Rotate(pDC, 90, false);
}

void CIKOLOKVIJUM2024View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM trans;
	trans.eM11 = sX;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = sY;
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2024View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{

	float angle_rad = angle * (3.14 / 180.0f);

	XFORM trans;
	trans.eM11 = cos(angle_rad);
	trans.eM12 = sin(angle_rad);
	trans.eM21 = -sin(angle_rad);
	trans.eM22 = cos(angle_rad);
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2024View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans;
	trans.eM11 = 1.0;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = 1.0;
	trans.eDx = dX;
	trans.eDy = dY;

	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2024View::DrawImageTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CIKOLOKVIJUM2024View::DrawBackground(CDC* pDC)
{
	int w = background->Width();
	int h = background->Height();

	CRect srcRect(0, 0, w, h);
	
	CRect clientRect;
	GetClientRect(&clientRect);

	CRect dstRect(0, 0, clientRect.Width(), clientRect.Height());

	background->Draw(pDC, srcRect, dstRect);
}

void CIKOLOKVIJUM2024View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2024Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	pMemDC->SelectObject(&bmp);
	int prevMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldForm;
	pMemDC->GetWorldTransform(&oldForm);
	// start
	DrawBackground(pMemDC);
	DrawCarousel(pMemDC, 150, 35, offset, alpha, beta, platformAngle);

	// end
	pMemDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
}


// CIKOLOKVIJUM2024View printing


void CIKOLOKVIJUM2024View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIKOLOKVIJUM2024View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKOLOKVIJUM2024View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKOLOKVIJUM2024View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIKOLOKVIJUM2024View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIKOLOKVIJUM2024View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIKOLOKVIJUM2024View diagnostics

#ifdef _DEBUG
void CIKOLOKVIJUM2024View::AssertValid() const
{
	CView::AssertValid();
}

void CIKOLOKVIJUM2024View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKOLOKVIJUM2024Doc* CIKOLOKVIJUM2024View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKOLOKVIJUM2024Doc)));
	return (CIKOLOKVIJUM2024Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKOLOKVIJUM2024View message handlers

BOOL CIKOLOKVIJUM2024View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return 1;
}

void CIKOLOKVIJUM2024View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	
	if (nChar == 'Q') {
		offset -= 5;
	}

	else if (nChar == 'W') {
		offset += 5;
	}
	
	else if (nChar == 'A') {
		alpha -= 5;
	}

	else if (nChar == 'S') {
		alpha += 5;
	}
	
	else if (nChar == 'D') {
		beta -= 5;
	}

	else if (nChar == 'F') {
		beta += 5;
	}
	
	else if (nChar == 'E') {
		platformAngle += 5;
	}

	else if (nChar == 'R') {
		platformAngle -= 5;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
