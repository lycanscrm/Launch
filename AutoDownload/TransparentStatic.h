#pragma once
#include "stdafx.h"

// CTransparentStatic

class CTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic)

public:
	CTransparentStatic();
	virtual ~CTransparentStatic();
	void SetTextColor(COLORREF);

protected:
   afx_msg LRESULT OnSetText(WPARAM,LPARAM);
   afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
   DECLARE_MESSAGE_MAP()

private:
	bool MTransparent = true;
	COLORREF MBackgroundColor = RGB(255, 255, 255);  // default is white (in case someone sets opaque without setting a color)
	COLORREF MTextColor = RGB(0, 0, 0);  // default is black. it would be more clean 

};


