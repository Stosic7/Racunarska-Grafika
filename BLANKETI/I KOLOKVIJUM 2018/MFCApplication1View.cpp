
// MFCApplication1View.cpp : implementation of the CMFCApplication1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCApplication1View construction/destruction

CMFCApplication1View::CMFCApplication1View() noexcept
{
	arm1 = new DImage();
	arm2 = new DImage();
	bager = new DImage();
	pozadina = new DImage();

	viljuska = GetEnhMetaFile(CString("slike/viljuska.emf"));

	arm1->Load(CString("slike/arm1.png"));
	arm2->Load(CString("slike/arm2.png"));
	bager->Load(CString("slike/bager.png"));
	pozadina->Load(CString("slike/pozadina.png"));
}

CMFCApplication1View::~CMFCApplication1View()
{
	delete arm1, arm2, bager, pozadina;
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View drawing

void CMFCApplication1View::DrawExcavator(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);

	pDC->SetWorldTransform(&oldForm);
}

void CMFCApplication1View::DrawFork(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Scale(pDC, 3.0, 3.0, false);
	Translate(pDC, -45, -113, false);
	Rotate(pDC, 89, false);
	
	Translate(pDC, 14, 20, false);
	Rotate(pDC, forkAngle, false);
	Translate(pDC, -14, -20, false);
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &header);

	int width = (header.rclFrame.right - header.rclFrame.left) / 26;
	int height = (header.rclFrame.bottom - header.rclFrame.top) / 26;

	CRect rect(0, 0, width, height);
	pDC->PlayMetaFile(viljuska, &rect);
	Rotate(pDC, -89, false);
	Translate(pDC, 45, 113, false);

}

void CMFCApplication1View::DrawArm2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 99, -270, false);
	Rotate(pDC, 180, false);
	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Angle, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);
	Rotate(pDC, -180, false);
	Translate(pDC, -99, 270, false);

}

void CMFCApplication1View::DrawArm1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Rotate(pDC, -90, false);
	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Angle, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
	Rotate(pDC, 90, false);
}

void CMFCApplication1View::DrawBody(CDC* pDC)
{
	DImage* img = bager;
	Translate(pDC, img->Width() / 2, img->Height(), false);
	DrawImgTransparent(pDC, img);
	Translate(pDC, -(img->Width() / 2), -(img->Height()), false);
	Translate(pDC, 185, 525, false);
}

void CMFCApplication1View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

void CMFCApplication1View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CMFCApplication1View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CMFCApplication1View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CMFCApplication1View::DrawBackground(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);

	int w = pozadina->Width();
	int h = pozadina->Height();

	CRect imgRect(0, 0, w, h);
	CRect imgDest(0, 0, clientRect.Width(), clientRect.Height());

	pozadina->Draw(pDC, imgRect, imgDest);
}

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
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
	//start
	
	DrawBackground(pMemDC);
	Translate(pMemDC, 1400 + excavatorX, 320, false);
	DrawExcavator(pMemDC);

	//end
	pMemDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
}


// CMFCApplication1View printing


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '1') {
		arm1Angle += 10.0f;
		if (arm1Angle > 90.0f) arm1Angle = 90.0f;
	}
	else if (nChar == '2') {
		arm1Angle -= 10.0f;
		if (arm1Angle < -90.0f) arm1Angle = -90.0f;
	} else if (nChar == '3') {
		arm2Angle += 10.0f;
		if (arm2Angle > 90.0f) arm2Angle = 90.0f;
	} else if (nChar == '4') {
		arm2Angle -= 10.0f;
		if (arm2Angle < -90.0f) arm2Angle = -90.0f;
	} else if (nChar == '5') {
		forkAngle += 10.0f;
		if (forkAngle > 90.0f) forkAngle = 90.0f;
	} else if (nChar == '6') {
		forkAngle -= 10.0f;
		if (forkAngle < -90.0f) forkAngle = -90.0f;
	} else if (nChar == VK_LEFT) {
		excavatorX -= 10.0f;
	} else if (nChar == VK_RIGHT) {
		excavatorX += 10.0f;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMFCApplication1View::OnEraseBkgnd(CDC* pDC)
{
	return 1;
}



// CMFCApplication1View diagnostics

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG

// CMFCApplication1View message handlers
