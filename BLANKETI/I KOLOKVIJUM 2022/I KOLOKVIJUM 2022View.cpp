
// IKOLOKVIJUM2022View.cpp : implementation of the CIKOLOKVIJUM2022View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKOLOKVIJUM2022.h"
#endif

#include "IKOLOKVIJUM2022Doc.h"
#include "IKOLOKVIJUM2022View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIKOLOKVIJUM2022View

IMPLEMENT_DYNCREATE(CIKOLOKVIJUM2022View, CView)

BEGIN_MESSAGE_MAP(CIKOLOKVIJUM2022View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIKOLOKVIJUM2022View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKOLOKVIJUM2022View construction/destruction

CIKOLOKVIJUM2022View::CIKOLOKVIJUM2022View() noexcept
{
	base = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	head = new DImage();
	pozadina = new DImage();
	base_shadow = new DImage();
	arm1_shadow = new DImage();
	arm2_shadow = new DImage();
	head_shadow = new DImage();

	base->Load(CString("slike/base.png"));
	arm1->Load(CString("slike/arm1.png"));
	arm2->Load(CString("slike/arm2.png"));
	head->Load(CString("slike/head.png"));
	pozadina->Load(CString("slike/pozadina.jpg"));
	base_shadow->Load(CString("slike/base_shadow.png"));
	arm1_shadow->Load(CString("slike/arm1_shadow.png"));
	arm2_shadow->Load(CString("slike/arm2_shadow.png"));
	head_shadow->Load(CString("slike/head_shadow.png"));
}

CIKOLOKVIJUM2022View::~CIKOLOKVIJUM2022View()
{
	delete base;
	delete arm1;
	delete arm2;
	delete head;
	delete pozadina;
	delete base_shadow;
	delete arm1_shadow;
	delete arm2_shadow;
	delete head_shadow;
}

BOOL CIKOLOKVIJUM2022View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKOLOKVIJUM2022View drawing



void CIKOLOKVIJUM2022View::DrawLampShadow(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Translate(pDC, 300, 1000, false);
	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);
	DrawLamp(pDC, true);

	pDC->SetWorldTransform(&xf);

}

void CIKOLOKVIJUM2022View::DrawLamp(CDC* pDC, bool bIsShadow)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	DrawLampBase(pDC, bIsShadow);
	DrawLampArm1(pDC, bIsShadow);
	DrawLampArm2(pDC, bIsShadow);
	DrawLampHead(pDC, bIsShadow);

	pDC->SetWorldTransform(&xf);
}

void CIKOLOKVIJUM2022View::DrawLampHead(CDC* pDC, bool bIsShadow)
{
	DImage* img = bIsShadow ? head_shadow : head;

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, 90, false);
	Translate(pDC, -450, -140, false);
	Translate(pDC, 178, 100, false);
	Rotate(pDC, headAngle, false);
	Translate(pDC, -178, -100, false);

	DrawImgTransparent(pDC, img);
}

void CIKOLOKVIJUM2022View::DrawLampArm2(CDC* pDC, bool bIsShadow)
{
	DImage* img = bIsShadow ? arm2_shadow : arm2;

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, -90, false);
	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Angle, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, img);
	Rotate(pDC, 90, false);
}

void CIKOLOKVIJUM2022View::DrawLampArm1(CDC* pDC, bool bIsShadow)
{
	DImage* img = bIsShadow ? arm1_shadow : arm1;

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, -45, false);
	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Angle, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, img);
	Translate(pDC, 270, 100, false);
}

void CIKOLOKVIJUM2022View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xf = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2022View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = angle * (3.14 / 180.0f);
	XFORM xf = { cos(rad), sin(rad), -sin(rad), cos(rad), 0, 0};
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2022View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xf = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2022View::DrawLampBase(CDC* pDC, bool bIsShadow)
{
	CRect cr;
	GetClientRect(&cr);
	DImage* img = bIsShadow ? base_shadow : base;

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Translate(pDC, ((cr.Width() / 2)), ((cr.Height() / 2) + 210), false); // pretpostavka da se app otvara u full window, kao na blanketu
	DrawImgTransparent(pDC, img);
	Translate(pDC, 90, 10, false);

}

void CIKOLOKVIJUM2022View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CIKOLOKVIJUM2022View::DrawBackground(CDC* pDC)
{
	CRect cr;
	GetClientRect(&cr);

	int w = pozadina->Width();
	int h = pozadina->Height();

	int x = (cr.Width() - w) / 2;
	int y = cr.Height() - h;

	CRect srcRect(0, 0, w, h);
	CRect dstRect(x, y, x + w, y + h);
	pozadina->Draw(pDC, srcRect, dstRect);
}

void CIKOLOKVIJUM2022View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2022Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cr;
	GetClientRect(&cr);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
	CBitmap* pOldbmp = pMemDC->SelectObject(&bmp);
	pMemDC->FillSolidRect(cr, pDC->GetBkColor());

	int prevMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	XFORM xf;
	pMemDC->GetWorldTransform(&xf);
	// start
	
	DrawBackground(pMemDC);
	DrawLampShadow(pMemDC);
	DrawLamp(pMemDC, false);

	// end
	pMemDC->SetWorldTransform(&xf);

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(&pOldbmp);
	pMemDC->SetGraphicsMode(prevMode);
	delete pMemDC;
}


// CIKOLOKVIJUM2022View printing


void CIKOLOKVIJUM2022View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIKOLOKVIJUM2022View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKOLOKVIJUM2022View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKOLOKVIJUM2022View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIKOLOKVIJUM2022View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIKOLOKVIJUM2022View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIKOLOKVIJUM2022View diagnostics

#ifdef _DEBUG
void CIKOLOKVIJUM2022View::AssertValid() const
{
	CView::AssertValid();
}

void CIKOLOKVIJUM2022View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKOLOKVIJUM2022Doc* CIKOLOKVIJUM2022View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKOLOKVIJUM2022Doc)));
	return (CIKOLOKVIJUM2022Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKOLOKVIJUM2022View message handlers

BOOL CIKOLOKVIJUM2022View::OnEraseBkgnd(CDC* pDC)
{

	return 1;
}

void CIKOLOKVIJUM2022View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == '1') {
		arm1Angle -= 10;
	}
	else if (nChar == '2') {
		arm1Angle += 10;

	} else if (nChar == '3') {
		arm2Angle -= 10;
	}
	else if (nChar == '4') {
		arm2Angle += 10;
	}
	else if (nChar == '5') {
		headAngle -= 10;
	}
	else if (nChar == '6') {
		headAngle += 10;
	}
	
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
