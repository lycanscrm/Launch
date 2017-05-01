// TransparentStatic.cpp : implementation file
//

#include "TransparentStatic.h"


// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic()
{
}

CTransparentStatic::~CTransparentStatic()
{
}
void CTransparentStatic::SetTextColor(COLORREF AColor)
{
	MTextColor = AColor;
	Invalidate();
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
   ON_MESSAGE(WM_SETTEXT,OnSetText)
   ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CTransparentStatic message handlers

LRESULT CTransparentStatic::OnSetText(WPARAM wParam,LPARAM lParam)
{
   LRESULT Result = Default();
   CRect Rect;
   GetWindowRect(&Rect);
   GetParent()->ScreenToClient(&Rect);
   GetParent()->InvalidateRect(&Rect);
   GetParent()->UpdateWindow();
   return Result;
}


HBRUSH CTransparentStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
   pDC->SetBkMode(TRANSPARENT);
   pDC->SetTextColor(MBackgroundColor);
   return (HBRUSH)GetStockObject(NULL_BRUSH);
}

