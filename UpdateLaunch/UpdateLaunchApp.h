#pragma once

#ifndef __AFXWIN_H__
#error ""
#endif

#include "resource.h"

class UpdateLaunch : public CWinApp
{
public:
	UpdateLaunch();
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
extern UpdateLaunch theApp;