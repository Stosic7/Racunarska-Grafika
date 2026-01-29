// I KOLOKVIJUM 2022View.cpp : implementation of the CIKOLOKVIJUM2022View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I KOLOKVIJUM 2022.h"
#endif

#include "I KOLOKVIJUM 2022Doc.h"
#include "I KOLOKVIJUM 2022View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef RAD
#define RAD(x) ((x) * 3.14159265358979323846f / 180.0f)
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
	// TODO: add construction code here
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
	delete base, arm1, arm2, head, pozadina, base_shadow, arm1_shadow, arm2_shadow, head_shadow;
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
	CRect clientRect;
	GetClientRect(&clientRect);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, clientRect.Width() / 2 + 127, clientRect.Height() / 2 + 225, false);

	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);

	Translate(pDC, -(clientRect.Width() / 2 + 127), -(clientRect.Height() / 2 + 225), false);
	DrawLamp(pDC, true);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2022View::DrawLamp(CDC* pDC, bool bIsShadow)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, clientRect.Width() / 2 + 125, clientRect.Height() / 2 + 275, false);
	DrawLampBase(pDC, bIsShadow);
	DrawLampArm1(pDC, bIsShadow);
	DrawLampArm2(pDC, bIsShadow);
	DrawLampHead(pDC, bIsShadow);
	Translate(pDC, -(clientRect.Width() / 2 + 125), -(clientRect.Height() / 2 + 275), false);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2022View::DrawLampHead(CDC* pDC, bool bIsShadow)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DImage* img = bIsShadow ? head_shadow : head;
	Translate(pDC, -9, -678, false);
	Rotate(pDC, 45, false);
	Translate(pDC, 178, 100, false);
	Rotate(pDC, headAngle, false);
	Translate(pDC, -178, -100, false);
	DrawImgTransparent(pDC, img);
	Rotate(pDC, -45, false);
	Translate(pDC, 9, 678, false);

}

void CIKOLOKVIJUM2022View::DrawLampArm2(CDC* pDC, bool bIsShadow)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DImage* img = bIsShadow ? arm2_shadow : arm2;
	Translate(pDC, 195, -245, false);
	Rotate(pDC, -130, false);
	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Angle, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, img);
	Rotate(pDC, 130, false);
	Translate(pDC, -195, 245, false);


}

void CIKOLOKVIJUM2022View::DrawLampArm1(CDC* pDC, bool bIsShadow)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DImage* img = bIsShadow ? arm1_shadow : arm1;
	Translate(pDC, -60, -120, false);
	Rotate(pDC, -45, false);
	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Angle, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, img);
	Rotate(pDC, 45, false);
	Translate(pDC, 60, 120, false);

}

void CIKOLOKVIJUM2022View::DrawLampBase(CDC* pDC, bool bIsShadow)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DImage* img = bIsShadow ? base_shadow : base;
	Translate(pDC, -img->Width() / 2, -img->Height(), false);
	DrawImgTransparent(pDC, img);
	Translate(pDC, img->Width() / 2, img->Height(), false);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2022View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF transparent = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	pImage->DrawTransparent(pDC, rect, rect, transparent);
}

void CIKOLOKVIJUM2022View::DrawBackground(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	int w = pozadina->Width();
	int h = pozadina->Height();

	CRect imgRect(0, 0, w, h);
	CRect imgDest(0, 0, clientRect.Width(), clientRect.Height());

	pozadina->Draw(pDC, imgRect, imgDest);
}

void CIKOLOKVIJUM2022View::Scale(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM trans;
	trans.eM11 = dX;
	trans.eM12 = 0.0;
	trans.eM21 = 0.0;
	trans.eM22 = dY;
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2022View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angle_rad = RAD(angle);

	XFORM trans;
	trans.eM11 = cos(angle_rad);
	trans.eM12 = sin(angle_rad);
	trans.eM21 = -sin(angle_rad);
	trans.eM22 = cos(angle_rad);
	trans.eDx = 0.0;
	trans.eDy = 0.0;

	pDC->ModifyWorldTransform(&trans, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2022View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CIKOLOKVIJUM2022View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2022Doc* pDoc = GetDocument();
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
	pMemDC->FillSolidRect(clientRect, pDC->GetBkColor());
	// call functions
	DrawBackground(pMemDC);
	DrawLampShadow(pMemDC);
	DrawLamp(pMemDC, false);
	// end of functions
	pMemDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
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

	switch (nChar) {
	case '1':
		arm1Angle -= 5.0;
		if (arm1Angle < -45.0) arm1Angle = -45.0;
		break;
	case '2':
		arm1Angle += 5.0;
		if (arm1Angle > 45.0) arm1Angle = 45.0;
		break;
	case '3':
		arm2Angle -= 5.0;
		if (arm2Angle < -130.0) arm2Angle = -130.0;
		break;
	case '4':
		arm2Angle += 5.0;
		if (arm2Angle > 130.0) arm2Angle = 130.0;
		break;
	case '5':
		headAngle -= 5.0;
		if (headAngle < -45.0) headAngle = -45.0;
		break;
	case '6':
		headAngle += 5.0;
		if (headAngle > 45.0) headAngle = 45.0;
		break;
	}
	
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
