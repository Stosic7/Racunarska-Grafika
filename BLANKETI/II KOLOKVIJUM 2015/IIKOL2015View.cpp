
// IIKOL2015View.cpp : implementation of the CIIKOL2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IIKOL2015.h"
#endif

#include "IIKOL2015Doc.h"
#include "IIKOL2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIIKOL2015View

IMPLEMENT_DYNCREATE(CIIKOL2015View, CView)

BEGIN_MESSAGE_MAP(CIIKOL2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIIKOL2015View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIIKOL2015View construction/destruction

CIIKOL2015View::CIIKOL2015View() noexcept
{
	// TODO: add construction code here

}

CIIKOL2015View::~CIIKOL2015View()
{
}

BOOL CIIKOL2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIIKOL2015View drawing

void CIIKOL2015View::OnDraw(CDC* pDC)
{
	CIIKOL2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CIIKOL2015View printing


void CIIKOL2015View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIIKOL2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIIKOL2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIIKOL2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIIKOL2015View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIIKOL2015View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

int CIIKOL2015View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}

void CIIKOL2015View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

void CIIKOL2015View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


// CIIKOL2015View diagnostics

#ifdef _DEBUG
void CIIKOL2015View::AssertValid() const
{
	CView::AssertValid();
}

void CIIKOL2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIIKOL2015Doc* CIIKOL2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIIKOL2015Doc)));
	return (CIIKOL2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CIIKOL2015View message handlers

BOOL CIIKOL2015View::OnEraseBkgnd(CDC* pDC)
{

	return 1;
}

void CIIKOL2015View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

void CIIKOL2015View::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CIIKOL2015View::OnLButtonUp(UINT nFlags, CPoint point)
{
}

void CIIKOL2015View::OnMouseMove(UINT nFlags, CPoint point)
{
}

BOOL CIIKOL2015View::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	return 0;
}

void CIIKOL2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT)          m_glRenderer.m_alpha -= 5.0;
	if (nChar == VK_LEFT)           m_glRenderer.m_alpha += 5.0;
	if (nChar == VK_UP)             m_glRenderer.m_beta += 5.0;
	if (nChar == VK_DOWN)           m_glRenderer.m_beta -= 5.0;
	if (nChar == VK_ADD)            m_glRenderer.m_dist /= 1.1;
	if (nChar == VK_SUBTRACT)       m_glRenderer.m_dist *= 1.1;
	if (nChar == 'Q')               m_glRenderer.m_moonRot -= 5.0;
	if (nChar == 'W')               m_glRenderer.m_moonRot += 5.0;
	if (nChar == 'S')               m_glRenderer.m_bLight = !m_glRenderer.m_bLight;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
