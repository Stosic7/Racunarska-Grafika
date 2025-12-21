#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "LukaStosic19424.h"
#endif

#include "LukaStosic19424Doc.h"
#include "LukaStosic19424View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CLukaStosic19424View, CView)

BEGIN_MESSAGE_MAP(CLukaStosic19424View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLukaStosic19424View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CLukaStosic19424View::CLukaStosic19424View() noexcept
{
	part = new DImage * [3];
	for (int i = 0; i < 3; i++)
		part[i] = new DImage[3];

	part[0][0].Load(CString("1.bmp"));
	part[0][1].Load(CString("2.bmp"));
	part[0][2].Load(CString("3.bmp"));
	part[1][0].Load(CString("4.bmp"));
	part[1][1].Load(CString("5.bmp"));
	part[1][2].Load(CString("6.bmp"));
	part[2][0].Load(CString("7.bmp"));
	part[2][1].Load(CString("8.bmp"));
	part[2][2].Load(CString("9.bmp"));

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (i == 1 && j == 2)
				MakeItBlue(&part[i][j]);
			else
				MakeItGray(&part[i][j]);
}

CLukaStosic19424View::~CLukaStosic19424View()
{
	for (int i = 0; i < 3; i++)
		delete[] part[i];
	delete[] part;
}

BOOL CLukaStosic19424View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CLukaStosic19424View::OnDraw(CDC* pDC)
{
	CLukaStosic19424Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientWindow;
	GetClientRect(&clientWindow);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientWindow.Width(), clientWindow.Height());

	int oldGMode = pDC->SetGraphicsMode(GM_ADVANCED);

	SetPuzzle(pDC, &bmp);

	delete memDC;
}

void CLukaStosic19424View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLukaStosic19424View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CLukaStosic19424View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CLukaStosic19424View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CLukaStosic19424View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLukaStosic19424View::OnContextMenu(CWnd* pWnd, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CLukaStosic19424View::Translate(CDC* pDC, float x, float y, bool rightMultiply)
{
	XFORM form =
	{
		1,
		0,
		0,
		1,
		x,
		y
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::Scale(CDC* pDC, float x, float y, bool rightMultiply)
{
	XFORM form =
	{
		x,
		0,
		0,
		y,
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form =
	{
		cosf(angle * stepAngle),
		sinf(angle * stepAngle),
		-sinf(angle * stepAngle),
		cosf(angle * stepAngle),
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form =
	{
		mx ? -1 : 1,
		0,
		0,
		my ? -1 : 1,
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLukaStosic19424View::DrawGrid(CDC* memDC)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(1, 1, 1));
	CPen* oldPen = memDC->SelectObject(pen);
	for (int p = 25; p <= 500; p = p + 25)
	{
		memDC->MoveTo(p, 0);
		memDC->LineTo(p, 500);
		memDC->MoveTo(0, p);
		memDC->LineTo(500, p);
	}
	memDC->SelectObject(oldPen);
	delete pen;
}

void CLukaStosic19424View::MakeItGray(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		int pixel = 0;
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;
			pixel += pixelArray[ptr + i];
		}
		pixel /= 3;
		pixel += 64;
		if (pixel > 255)
			pixel = 255;

		for (int i = 0; i < 3 && !bg; i++)
			pixelArray[ptr + i] = pixel;
	}
	img->Update();
}

void CLukaStosic19424View::MakeItBlue(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;
		}

		for (int i = 0; i < 3 && !bg; i++)
		{
			if (i == 0)
				pixelArray[ptr + i] = pixelArray[ptr + i];
			else
				pixelArray[ptr + i] = 0;
		}
	}
	img->Update();
}

void CLukaStosic19424View::MemDCTopDC(CDC* memDC, CDC* pDC)
{
	CRect clientWindow;
	GetClientRect(&clientWindow);

	CBitmap mask;
	mask.CreateBitmap(clientWindow.Width(), clientWindow.Height(), 1, 1, NULL);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(memDC);
	dstDC->SelectObject(&mask);

	memDC->SetBkColor(RGB(0, 0, 0));
	memDC->SetTextColor(RGB(255, 255, 255));

	dstDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCCOPY);
	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), dstDC, 0, 0, SRCAND);
	delete dstDC;
	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCPAINT);
}

void CLukaStosic19424View::MakeItRed(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];

	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;
		}

		for (int i = 0; i < 3 && !bg; i++)
		{
			if (i == 2)
				pixelArray[ptr + i] = pixelArray[ptr + i];
			else
				pixelArray[ptr + i] = 0;
		}
	}
	img->Update();
}

void CLukaStosic19424View::DrawTransparent(CDC* pDC, DImage* img, int x, int y)
{
	CBitmap mask;
	mask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);

	CBitmap image;
	image.CreateBitmap(img->Width(), img->Height(), 4, 8, NULL);
	image.SetBitmapBits(img->Width() * img->Height() * img->BPP(), img->GetDIBBits());

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
	CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);

	COLORREF removeColor = srcDC->GetPixel(0, 0);
	COLORREF srcOldBckColor = srcDC->SetBkColor(removeColor);

	dstDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);

	COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, img->Width(), img->Height(), dstDC, 0, 0, SRCAND);

	dstDC->SelectObject(dstOldBitmap);
	delete dstDC;

	srcDC->SelectObject(&mask);
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCAND);

	srcDC->SelectObject(&image);
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCPAINT);
	srcDC->SelectObject(srcOldBitmap);
	delete srcDC;
}

void CLukaStosic19424View::SetPuzzle(CDC* pDC, CBitmap* bmp)
{
	pDC->SelectObject(bmp);
	DrawGrid(pDC);

	int oldGMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 128 - 22, 128 - 22, false);
	Mirror(pDC, true, false, false);
	Rotate(pDC, 205, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][0], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 150, 0, false);
	Translate(pDC, 128 - 22, 125 - 22, false);
	Rotate(pDC, 125, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[2][0], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 2 * 150, 0, false);
	Translate(pDC, 128 - 22, 128 - 32, false);
	Rotate(pDC, 76, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][1], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 0, 150, false);
	Translate(pDC, 128 - 38, 128 - 35, false);
	Rotate(pDC, 253, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(116), -(128), false);

	DrawTransparent(pDC, &part[2][2], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 150, 150, false);
	Translate(pDC, 128 - 39, 128 - 31, false);
	Rotate(pDC, -58, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][1], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 2 * 150, 150, false);
	Translate(pDC, 128 - 38, 128 - 28, false);
	Rotate(pDC, -72, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][2], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 0, 2 * 150, false);
	Translate(pDC, 128 - 33, 128 - 35, false);
	Rotate(pDC, -19, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][0], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 150, 2 * 150, false);
	Translate(pDC, 128 - 36, 128 - 23, false);
	Rotate(pDC, -113, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][2], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	Translate(pDC, 2 * 150, 2 * 150, false);
	Translate(pDC, 128 - 30, 128 - 20, false);
	Rotate(pDC, 199, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[2][1], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	pDC->SetGraphicsMode(oldGMode);
}

void CLukaStosic19424View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

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
#endif
