#include "stdafx.h"
#include "UpdateLaunchApp.h"
#include "UpdateLaunch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(UpdateLaunch, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

UpdateLaunch::UpdateLaunch()
{
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

UpdateLaunch theApp;

BOOL UpdateLaunch::InitInstance()
{
	CWinApp::InitInstance();
	UpdateLaunchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	return FALSE;
}