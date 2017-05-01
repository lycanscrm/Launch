
#include "resource.h"    //main symbols
class CHot : public CBitmapButton
{
	DECLARE_DYNAMIC(CHot)

public:
	CHot();
	virtual ~CHot();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


