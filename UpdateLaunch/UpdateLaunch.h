#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//-----------------------------------------------------------------------------------------
//Globals
class UpdateLaunchDlg : public CDialog
{
public:
	UpdateLaunchDlg(CWnd* pParent = NULL) : CDialog(UpdateLaunchDlg::IDD, pParent) {    }
	// Dialog Data, name of dialog form
	enum{ IDD = IDD_DIALOG1 };
	char *LaunchINI = "../Launch.ini"; // File Launch.ini
	char *FVersion = "./(LaunchVersion)"; // File (LaunchVersion)
	BOOL DownloadFile(char* u);
	void SetProgress(int to);
	void Update();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
private:
	HWND m_hNotifyWnd;
public:
	afx_msg void OnTimer(UINT_PTR nTimerID);

	CProgressCtrl m_progress1;
	HINSTANCE hInstance;

	char inBuf[MAX_PATH];
	char* url;
	char* file_name;

	CString cstring;

	CStatic m_MESSAGE;
	CEdit m_FileUrl;
	CEdit m_UnpackPath;

	DECLARE_MESSAGE_MAP()
};
UINT CURLDownload(LPVOID pParam);
UINT Unpack(LPVOID pParam);