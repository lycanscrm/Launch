//-----------------------------------------------------------------------------------------
#include "stdafx.h"
#include <afxwin.h>
#include "resource.h"

class TheGame : public CWinApp
{
public:
	TheGame() {  }
public:
	virtual BOOL InitInstance()
	{
		ShellExecute(NULL, TEXT("Open"), TEXT("UpdateLaunch.exe"), NULL, TEXT("Bin"), SW_SHOW);
		return FALSE;
	}
};
TheGame theApp;