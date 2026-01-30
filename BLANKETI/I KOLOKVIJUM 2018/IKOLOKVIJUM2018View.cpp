
// IKOLOKVIJUM2018View.cpp : implementation of the CIKOLOKVIJUM2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKOLOKVIJUM2018.h"
#endif

#include "IKOLOKVIJUM2018Doc.h"
#include "IKOLOKVIJUM2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIKOLOKVIJUM2018View

IMPLEMENT_DYNCREATE(CIKOLOKVIJUM2018View, CView)

BEGIN_MESSAGE_MAP(CIKOLOKVIJUM2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIKOLOKVIJUM2018View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKOLOKVIJUM2018View construction/destruction

CIKOLOKVIJUM2018View::CIKOLOKVIJUM2018View() noexcept
{
	bager = new DImage();
	arm1 = new DImage();;
	arm2 = new DImage();
	pozadina = new DImage();
	
	bager->Load(CString("slike/bager.png"));
	arm1->Load(CString("slike/arm1.png"));
	arm2->Load(CString("slike/arm2.png"));
	pozadina->Load(CString("slike/pozadina.png"));
	viljuska = GetEnhMetaFile(CString("slike/viljuska.emf"));
}

CIKOLOKVIJUM2018View::~CIKOLOKVIJUM2018View()
{
	delete bager, arm1, arm2, pozadina;
	DeleteEnhMetaFile(viljuska);
}

BOOL CIKOLOKVIJUM2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKOLOKVIJUM2018View drawing

void CIKOLOKVIJUM2018View::DrawExcavator(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);

	pDC->SetWorldTransform(&xf);
}

void CIKOLOKVIJUM2018View::DrawFork(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, 180, false);
	Scale(pDC, 3.5, 3.5, false);
	Rotate(pDC, 90, false);
	Translate(pDC, 7, 12, false);
	Rotate(pDC, forkAngle, false);
	Translate(pDC, -7, -12, false);
	pDC->PlayMetaFile(viljuska, CRect(0, 0, 44, 34));
}

void CIKOLOKVIJUM2018View::DrawArm2(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, -90, false);
	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Angle, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);
	Translate(pDC, 230, 69, false);
}

void CIKOLOKVIJUM2018View::DrawArm1(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Rotate(pDC, -90, false);
	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Angle, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
	Translate(pDC, 268, 100, false);
}

void CIKOLOKVIJUM2018View::DrawBody(CDC* pDC)
{
	CRect cr;
	GetClientRect(&cr);
	DImage* img = bager;

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Translate(pDC, (cr.Width() - img->Width()), (cr.Height() - img->Height()), false);
	DrawImgTransparent(pDC, img);
	Translate(pDC, 1, 230, false);
}

void CIKOLOKVIJUM2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect cr(0, 0, w, h);

	pImage->DrawTransparent(pDC, cr, cr, color);
}

void CIKOLOKVIJUM2018View::DrawBackground(CDC* pDC)
{
	CRect cr;
	GetClientRect(&cr);

	int imgWidth = pozadina->Width();
	int imgHeight = pozadina->Height();

	int x = (cr.Width() - imgWidth) / 2;
	int y = cr.Height() - imgHeight;

	CRect srcRect(0, 0, imgWidth, imgHeight);
	CRect destRect(x, y, x + imgWidth, y + imgHeight);

	pozadina->Draw(pDC, srcRect, destRect);
}

void CIKOLOKVIJUM2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xf = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = angle * (3.14 / 180.0f);
	XFORM xf = { cos(rad), sin(rad), -sin(rad), cos(rad), 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xf = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2018View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cr;
	GetClientRect(&cr);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
	CBitmap* oldBmp = pMemDC->SelectObject(&bmp);
	pMemDC->FillSolidRect(cr, pDC->GetBkColor());
	int prevMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldForm;
	pMemDC->GetWorldTransform(&oldForm);
	// start
	DrawBackground(pMemDC);
	Translate(pMemDC, move, 0, false);
	DrawExcavator(pMemDC);
	//end
	pMemDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(&oldBmp);
	pMemDC->SetGraphicsMode(prevMode);
	delete pMemDC;
}


// CIKOLOKVIJUM2018View printing


void CIKOLOKVIJUM2018View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIKOLOKVIJUM2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKOLOKVIJUM2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKOLOKVIJUM2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIKOLOKVIJUM2018View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIKOLOKVIJUM2018View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIKOLOKVIJUM2018View diagnostics

#ifdef _DEBUG
void CIKOLOKVIJUM2018View::AssertValid() const
{
	CView::AssertValid();
}

void CIKOLOKVIJUM2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKOLOKVIJUM2018Doc* CIKOLOKVIJUM2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKOLOKVIJUM2018Doc)));
	return (CIKOLOKVIJUM2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKOLOKVIJUM2018View message handlers

BOOL CIKOLOKVIJUM2018View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return 1;
}

void CIKOLOKVIJUM2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar == '1') {
		arm1Angle -= 10.0;
	}
	else if (nChar == '2') {
		arm1Angle += 10.0;
	}
	else if (nChar == '3') {
		arm2Angle -= 10.0;
	}
	else if (nChar == '4') {
		arm2Angle += 10.0;
	}
	else if (nChar == '5') {
		forkAngle -= 10.0;
	}
	else if (nChar == '6') {
		forkAngle += 10.0;
	}
	else if (nChar == VK_LEFT) {
		move -= 10.0;
	}
	else if (nChar == VK_RIGHT) {
		move += 10.0;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
