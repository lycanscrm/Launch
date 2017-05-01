//
// Hot.cpp : implementation file
//

#include "stdafx.h"
#include "Hot.h"


// CHot

IMPLEMENT_DYNAMIC(CHot, CBitmapButton)

CHot::CHot()
{

}

CHot::~CHot()
{
}


BEGIN_MESSAGE_MAP(CHot, CBitmapButton)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CHot message handlers



void CHot::OnMouseHover(UINT nFlags, CPoint point)
{
	//MessageBox(L"hover",0,0);
	// TODO: Add your message handler code here and/or call default
	this->LoadBitmaps(IDB_BITMAP10, IDB_BITMAP10, 0, IDB_BITMAP8);
	this->RedrawWindow();

	CButton::OnMouseHover(nFlags, point);
}

void CHot::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_LEAVE|TME_HOVER;
    tme.dwHoverTime = 1;
    TrackMouseEvent(&tme);

	// TODO: Add your message handler code here and/or call default

	CButton::OnMouseMove(nFlags, point);
}

void CHot::OnMouseLeave()
{
	this->LoadBitmaps(IDB_BITMAP7, IDB_BITMAP10, 0, IDB_BITMAP8);
	this->RedrawWindow();
	// TODO: Add your message handler code here and/or call default

	CButton::OnMouseLeave();
}
