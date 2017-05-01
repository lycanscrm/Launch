#include "stdafx.h"
#include "Hot.h"
#include "resource.h"    //main symbols
#include "TransparentStatic.h"
#include "explorer1.h"

//-----------------------------------------------------------------------------------------
//Globals
class AutoDownloadDlg : public CDialog {
public:
	AutoDownloadDlg(CWnd* pParent = NULL) : CDialog(AutoDownloadDlg::IDD, pParent) {    }
	// Dialog Data, name of dialog form
	enum{ IDD = MAIN_LAUNCH };
	char *LaunchINI = "..\\Launch.ini"; // File Launch.ini
	char *FVersion = "..\\(version)"; // File (version)
	char *FPatchInfo = "..\\Patch\\patchinfo.txt"; // File patchinfo.txt
	int SetBitmap(UINT nIDResource);
	void ShowStartGameBTN();
	BOOL DownloadFile(char* u);
	void Update();
	void ShowVersion();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//Called right after constructor. Initialize things here.
	virtual BOOL OnInitDialog();
	//Disable ESC key close dialog
	BOOL PreTranslateMessage(MSG* pMsg);
public:

	CBitmap m_bitmap;
	int m_style;
	CHot btnBmp;

	CTransparentStatic m_MESSAGE;
	CTransparentStatic m_VERSION;
	CTransparentStatic m_ProgressText;
	CTransparentStatic m_DownSpeed;

	CEdit m_FileUrl;
	CEdit m_UnpackPath;

	CTransparentStatic m_HOME;
	CTransparentStatic m_CONFIG;
	CTransparentStatic m_REGISTER;
	CTransparentStatic m_CASH;
	CTransparentStatic m_FANPAGE;

	CExplorer1 m_browser;


	CString cstring;

	CProgressCtrl m_progress1;
	CProgressCtrl m_progress2;

	void SetProgress1(int to);
	void SetProgress2(int to);
	char v[MAX_PATH];
	bool needupdate = FALSE;

	afx_msg void OnTimer(UINT_PTR nTimerID);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMin();
	afx_msg void GoHOME();
	afx_msg void GoREG();
	afx_msg void GoCASH();
	afx_msg void GoFANPAGE();
	afx_msg void GoCONFIG();
	afx_msg void OnBnClickedStartGame();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};