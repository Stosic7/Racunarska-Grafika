
// IKOLOKVIJUM2023View.cpp : implementation of the CIKOLOKVIJUM2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKOLOKVIJUM2023.h"
#endif

#include "IKOLOKVIJUM2023Doc.h"
#include "IKOLOKVIJUM2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIKOLOKVIJUM2023View

IMPLEMENT_DYNCREATE(CIKOLOKVIJUM2023View, CView)

BEGIN_MESSAGE_MAP(CIKOLOKVIJUM2023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIKOLOKVIJUM2023View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKOLOKVIJUM2023View construction/destruction

CIKOLOKVIJUM2023View::CIKOLOKVIJUM2023View() noexcept
{
	glava = new DImage();
	nadkolenica = new DImage();
	nadlaktica = new DImage();
	podkolenica = new DImage();
	podlaktica = new DImage();
	saka = new DImage();
	stopalo = new DImage();
	telo = new DImage();
	pozadina = new DImage();

	glava->Load(CString("slike/glava.png"));
	nadkolenica->Load(CString("slike/nadkolenica.png"));
	nadlaktica->Load(CString("slike/nadlaktica.png"));
	podkolenica->Load(CString("slike/podkolenica.png"));
	podlaktica->Load(CString("slike/podlaktica.png"));
	saka->Load(CString("slike/saka.png"));
	stopalo->Load(CString("slike/stopalo.png"));
	telo->Load(CString("slike/telo.png"));
	pozadina->Load(CString("slike/pozadina.jpg"));

}

CIKOLOKVIJUM2023View::~CIKOLOKVIJUM2023View()
{
	delete glava;
	delete nadkolenica;
	delete nadlaktica;
	delete podkolenica;
	delete podlaktica;
	delete saka;
	delete stopalo;
	delete telo;
	delete pozadina;
}

BOOL CIKOLOKVIJUM2023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKOLOKVIJUM2023View drawing

void CIKOLOKVIJUM2023View::DrawRobot(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DrawHalf(pDC);
	Translate(pDC, 500, 160, false);
	Scale(pDC, -1, 1, false);
	Translate(pDC, -510, -160, false);
	DrawHalf(pDC);

	pDC->SetWorldTransform(&oldForm);

	DrawHead(pDC);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2023View::DrawHead(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 457, 58, false);
	DrawImgTransparent(pDC, glava);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2023View::DrawHalf(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm); // 0,0

	Translate(pDC, 400, 160, false); // vracamo se posle opet ovde kad crtamo noge
	DrawImgTransparent(pDC, telo);

	Translate(pDC, 25, 65, false);
	Rotate(pDC, nadlakticaAngle, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nadlaktica);

	Translate(pDC, 22, 167, false);
	Rotate(pDC, podlakticaAngle, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, podlaktica);

	Translate(pDC, 30, 140, false);
	Rotate(pDC, sakaAngle, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&oldForm);
	Translate(pDC, 400, 160, false);

	Translate(pDC, 61, 262, false);
	Translate(pDC, -29, -20, false);
	DrawImgTransparent(pDC, nadkolenica);
	
	Translate(pDC, 30, 184, false);
	Translate(pDC, -25, -37, false);
	DrawImgTransparent(pDC, podkolenica);

	Translate(pDC, 25, 248, false);
	Translate(pDC, -20, -16, false);
	DrawImgTransparent(pDC, stopalo);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2023View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xf = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = angle * (3.14 / 180.0f);
	XFORM xf = { cos(rad), sin(rad), -sin(rad), cos(rad), 0, 0};
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xf = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xf, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);
	int w = pImage->Width();
	int h = pImage->Height();
	CRect cr(0, 0, w, h);

	pImage->DrawTransparent(pDC, cr, cr, color);
}

void CIKOLOKVIJUM2023View::DrawBackground(CDC* pDC)
{
	int w = pozadina->Width();
	int h = pozadina->Height();
	CRect rect(0, 0, w, h);

	pozadina->Draw(pDC, rect, rect);
}

void CIKOLOKVIJUM2023View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cr;
	GetClientRect(&cr);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
	CBitmap* pOldBmp = pMemDC->SelectObject(&bmp);
	pMemDC->FillSolidRect(cr, pDC->GetBkColor());

	int prevMode = pMemDC->SetGraphicsMode(GM_ADVANCED);
	XFORM xf;
	pMemDC->GetWorldTransform(&xf);
	//start
	DrawBackground(pMemDC);
	Translate(pMemDC, 520, 470, false);
	Rotate(pMemDC, ceo, false);
	Scale(pMemDC, robotScale, robotScale, false);
	Translate(pMemDC, -520, -470, false);
	DrawRobot(pMemDC);
	//end
	pMemDC->SetWorldTransform(&xf);

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(&pOldBmp);
	pMemDC->SetGraphicsMode(prevMode);
	delete pMemDC;
}


// CIKOLOKVIJUM2023View printing


void CIKOLOKVIJUM2023View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIKOLOKVIJUM2023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKOLOKVIJUM2023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKOLOKVIJUM2023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIKOLOKVIJUM2023View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIKOLOKVIJUM2023View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIKOLOKVIJUM2023View diagnostics

#ifdef _DEBUG
void CIKOLOKVIJUM2023View::AssertValid() const
{
	CView::AssertValid();
}

void CIKOLOKVIJUM2023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKOLOKVIJUM2023Doc* CIKOLOKVIJUM2023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKOLOKVIJUM2023Doc)));
	return (CIKOLOKVIJUM2023Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKOLOKVIJUM2023View message handlers

BOOL CIKOLOKVIJUM2023View::OnEraseBkgnd(CDC* pDC)
{
	return 1;
}

void CIKOLOKVIJUM2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'A') {
		sakaAngle -= 10.0;
		if (sakaAngle < -10.0) sakaAngle = -10.0;
	}
	else if (nChar == 'S') {
		sakaAngle += 10.0;
		if (sakaAngle > 30.0) sakaAngle = 30.0;
	}
	else if (nChar == 'D') {
		podlakticaAngle -= 10.0;
		if (podlakticaAngle < -10.0) podlakticaAngle = -10.0;
	}
	else if (nChar == 'F') {
		podlakticaAngle += 10.0;
		if (podlakticaAngle > 80.0) podlakticaAngle = 80.0;
	}
	else if (nChar == 'G') {
		nadlakticaAngle -= 10.0;
		if (nadlakticaAngle < -10.0) nadlakticaAngle = -10.0;
	}
	else if (nChar == 'H') {
		nadlakticaAngle += 10.0;
		if (nadlakticaAngle > 90.0) nadlakticaAngle = 90.0;
	}
	else if (nChar == '1') {
		ceo -= 5.0f;
		robotScale -= 0.1;
	}
	else if (nChar == '2') {
		ceo += 5.0f;
		robotScale += 0.1;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
