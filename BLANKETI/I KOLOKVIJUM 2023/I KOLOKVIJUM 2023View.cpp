
// I KOLOKVIJUM 2023View.cpp : implementation of the CIKOLOKVIJUM2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I KOLOKVIJUM 2023.h"
#endif

#include "I KOLOKVIJUM 2023Doc.h"
#include "I KOLOKVIJUM 2023View.h"

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
	// TODO: add construction code here
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

void CIKOLOKVIJUM2023View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float rad = angle * 3.14159265f / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	XFORM xform = { cosA, sinA, -sinA, cosA, 0, 0 };
	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::DrawHead(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, ((pozadina->Width() / 2) / 2) + 116, ((pozadina->Height() / 2) / 2) - 220, false);
	DrawImgTransparent(pDC, glava);
	Translate(pDC, -(((pozadina->Width() / 2) / 2) + 75), -(((pozadina->Height() / 2) / 2) - 150), false);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2023View::DrawRobot(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	float centerX = ((pozadina->Width() / 2) / 2) + 60;

	DrawHalf(pDC);

	Translate(pDC, centerX + 210, 0, false);
	Mirror(pDC, -1, 1, false);
	Translate(pDC, -centerX, 0, false); 

	DrawHalf(pDC);
	DrawHead(pDC);

	pDC->SetWorldTransform(&oldForm);
}

void CIKOLOKVIJUM2023View::Mirror(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::DrawHalf(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, ((pozadina->Width() / 2) / 2) + 60, ((pozadina->Height() / 2) / 2) - 120, false); // telo krece od centra slike
	DrawImgTransparent(pDC, telo);

	Translate(pDC, 25, 65, false);
	Rotate(pDC, nadLakticaAngle, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nadlaktica);

	Translate(pDC, 22, 167, false);
	Rotate(pDC, podlakticaAngle, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, podlaktica);

	Translate(pDC, 30, 140, false);
	Rotate(pDC, sakaAngle, false);
	Translate(pDC, -25, 3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&oldForm);
	Translate(pDC, ((pozadina->Width() / 2) / 2) + 60, ((pozadina->Height() / 2) / 2) - 120, false);

	Translate(pDC, 61, 262, false);
	Translate(pDC, -29, 20, false);
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
	XFORM xform = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKOLOKVIJUM2023View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* buffer = pImage->GetDIBBits();
	COLORREF transparentColor = RGB(buffer[2], buffer[1], buffer[0]);

	CRect rc(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, rc, rc, transparentColor);
}

void CIKOLOKVIJUM2023View::DrawBackground(CDC* pDC)
{
	int bgWidth = pozadina->Width();
	int bgHeight = pozadina->Height();

	CRect clientRect;
	GetClientRect(&clientRect);

	CRect srcRect(0, 0, bgWidth, bgHeight);
	CRect dstRect(0, 0, (clientRect.Width()), (clientRect.Height()));

	pozadina->Draw(pDC, srcRect, dstRect);
}

void CIKOLOKVIJUM2023View::OnDraw(CDC* pDC)
{
	CIKOLOKVIJUM2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());

	CBitmap* oldBitmap = pMemDC->SelectObject(&memBitmap);
	int oldMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	XFORM oldForm;
	pMemDC->GetWorldTransform(&oldForm);
	// start
	
	DrawBackground(pMemDC);
	Rotate(pMemDC, ceo, false);
	Scale(pMemDC, robotScale, robotScale, false);
	DrawRobot(pMemDC);

	//end
	pMemDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(&oldBitmap);
	pMemDC->SetGraphicsMode(oldMode);

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
	// TODO: Add your message handler code here and/or call default

	return 1;
}

void CIKOLOKVIJUM2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == 'A' || nChar == 'a') && sakaAngle >= 0)
	{
		sakaAngle -= 5;
	}
	else if ((nChar == 'S' || nChar == 's') && sakaAngle <= 20)
	{
		sakaAngle += 5;
	}
	else if ((nChar == 'D' || nChar == 'd') && podlakticaAngle >= 0)
	{
		podlakticaAngle -= 5;
	}
	else if ((nChar == 'F' || nChar == 'f') && podlakticaAngle <= 70)
	{
		podlakticaAngle += 5;
	}
	else if ((nChar == 'G' || nChar == 'g') && nadLakticaAngle >= 0)
	{
		nadLakticaAngle -= 5;
	}
	else if ((nChar == 'H' || nChar == 'h') && nadLakticaAngle <= 80)
	{
		nadLakticaAngle += 5;
	}

	if (nChar == '1')
		robotScale += 0.1;
	else if (nChar == '2')
		robotScale -= 0.1;


	if (nChar == '3')
		ceo += 5;
	else if (nChar == '4')
		ceo -= 5;


	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
