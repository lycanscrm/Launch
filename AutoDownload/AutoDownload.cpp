#include "AutoDownload.h"
#include <wininet.h>
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------------------
//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(AutoDownloadDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STATIC, &AutoDownloadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_STATIC2, &AutoDownloadDlg::OnBnClickedMin)
	ON_BN_CLICKED(IDC_STATIC5, &AutoDownloadDlg::GoHOME)
	ON_BN_CLICKED(IDC_STATIC10, &AutoDownloadDlg::GoCONFIG)
	ON_BN_CLICKED(IDC_STATIC7, &AutoDownloadDlg::GoREG)
	ON_BN_CLICKED(IDC_STATIC8, &AutoDownloadDlg::GoCASH)
	ON_BN_CLICKED(IDC_STATIC9, &AutoDownloadDlg::GoFANPAGE)
	ON_BN_CLICKED(IDC_BUTTON1, &AutoDownloadDlg::OnBnClickedStartGame)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------
void AutoDownloadDlg::OnTimer(UINT_PTR nTimerID)
{
	KillTimer(nTimerID);
	CString cstring;
	char inBuf[MAX_PATH];

	//Client version
	ShowVersion();

	if(nTimerID == 1) {
		//Download txt file
		GetPrivateProfileStringA("Launch", "LoginServer", "", inBuf, MAX_PATH, LaunchINI);
		DownloadFile(inBuf);
	}

	if(nTimerID == 3) {
		// Add Browser
		GetPrivateProfileStringA("Launch", "Help_URL", "", inBuf, MAX_PATH, LaunchINI);
		//Reset cache
		SYSTEMTIME st;
		GetSystemTime(&st);
		LONG time_ms = (st.wYear*31556926) + (st.wMonth*2629743) + (st.wDay*86400) + (st.wHour*3600) + (st.wMinute*60) + st.wSecond;
		char strLong[MAX_PATH];
		sprintf(strLong, "%ld", time_ms);
		strcat(inBuf, "?");
		strcat(inBuf, strLong);
		//Reset cache END
		cstring = inBuf;
		m_browser.Navigate(cstring, NULL, NULL, NULL, NULL);
	}
	if (nTimerID == 100) {
		if (needupdate) {
			Update();
		} else {
			ShowStartGameBTN();
		}
	}

}
/////////////////////////////////////////////////////////////////////////////
// AutoDownloadDlg message handlers
BOOL AutoDownloadDlg::OnEraseBkgnd(CDC* pDC)
{
	//CDialog::OnEraseBkgnd(pDC);
	if (!m_bitmap.m_hObject)
		return true;

	CRect rect;
	GetClientRect(&rect);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = dc.SelectObject(&m_bitmap);
	BITMAP bmap;
	m_bitmap.GetBitmap(&bmap);

	CRect TargetRect;                // Region on screen to be updated
	pDC->GetClipBox(&TargetRect);
	TargetRect.IntersectRect(TargetRect, rect);
	CRect SrcRect;                    // Region from bitmap to be painted
	SrcRect.left = MulDiv(TargetRect.left, bmap.bmWidth, rect.Width());
	SrcRect.top = MulDiv(TargetRect.top, bmap.bmHeight, rect.Height());
	SrcRect.right = MulDiv(TargetRect.right, bmap.bmWidth, rect.Width());
	SrcRect.bottom = MulDiv(TargetRect.bottom, bmap.bmHeight, rect.Height());
	
	pDC->StretchBlt(TargetRect.left, TargetRect.top, TargetRect.Width(), TargetRect.Height(),
		&dc,
		SrcRect.left, SrcRect.top, SrcRect.Width(), SrcRect.Height(),
		SRCCOPY);

	dc.SelectObject(pOldBitmap);

	return true;
}

int AutoDownloadDlg::SetBitmap(UINT nIDResource)
{
	if (m_bitmap.LoadBitmap(nIDResource))
		return 0;
	else
		return 1;//error
}

BOOL AutoDownloadDlg::PreTranslateMessage(MSG* pMsg){
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}
	if (pMsg->message == WM_MOUSEMOVE && (pMsg->wParam & MK_LBUTTON))
	{
		CPoint p = pMsg->pt;
		ScreenToClient(&p);
		CRect r(0, 0, 800, 60);
		if (r.PtInRect(p))
		{
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
			SendMessage(WM_NCLBUTTONUP, HTCAPTION, 0);
			//this->Redrawbackground();
			return 1;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void AutoDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, MESSAGE, m_MESSAGE);
	DDX_Control(pDX, IDC_STATIC6, m_VERSION);
	DDX_Control(pDX, IDC_EXPLORER1, m_browser);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_PROGRESS_TEXT, m_ProgressText);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_DownSpeed);
	DDX_Control(pDX, IDC_STATIC5, m_HOME);
	DDX_Control(pDX, IDC_STATIC10, m_CONFIG);
	DDX_Control(pDX, IDC_STATIC7, m_REGISTER);
	DDX_Control(pDX, IDC_STATIC8, m_CASH);
	DDX_Control(pDX, IDC_STATIC9, m_FANPAGE);
	DDX_Control(pDX, IDC_BUTTON1, btnBmp);
	DDX_Control(pDX, IDC_EDIT_URL, m_FileUrl);
	DDX_Control(pDX, IDC_EDIT_UNPACK_PATH, m_UnpackPath);
}
BOOL AutoDownloadDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message){
	if (pWnd->GetDlgCtrlID() == IDC_BUTTON1){
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
BOOL AutoDownloadDlg::OnInitDialog(){
	CDialog::OnInitDialog();
	
	this->SetWindowText(TEXT("Launch"));

	//Set app icon
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(ICON_LAUNCH));
	SetIcon(hIcon, FALSE);

	//Set app Background
	SetBitmap(BACKGROUND_LAUNCH);

	//Open windows before do bellow
	ShowWindow(SW_SHOW);

	//Start Game Button
	btnBmp.LoadBitmaps(IDB_BITMAP7, IDB_BITMAP10, 0, IDB_BITMAP8);
	BITMAP bmap;
	CBitmap cbitmap;
	cbitmap.LoadBitmap(IDB_BITMAP7);
	cbitmap.GetBitmap(&bmap);
	btnBmp.SetWindowPos(NULL, 0, 0, bmap.bmWidth, bmap.bmHeight, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	btnBmp.SubclassDlgItem(IDC_BUTTON1, this);
	
	m_HOME.SetWindowText(TEXT("TRANG CHỦ"));
	m_CONFIG.SetWindowText(TEXT("CÀI ĐẶT"));
	m_REGISTER.SetWindowText(TEXT("ĐĂNG KÝ"));
	m_CASH.SetWindowText(TEXT("NẠP THẺ"));
	m_FANPAGE.SetWindowText(TEXT("FANPAGE"));

	SetTimer(1, 0, NULL);
	
	SetTimer(3, 500, NULL);

	return true;
}
void AutoDownloadDlg::ShowVersion() {
	CString cstring;
	char inBuf[MAX_PATH];
	FILE* f;
	f = fopen(FVersion, "r");
	fgets(v, sizeof(v), f);
	fclose(f);
	GetPrivateProfileStringA("Version", "Latest", "", inBuf, MAX_PATH, FPatchInfo);
	cstring = _T("Version hiện tại: ");
	cstring += v;
	cstring += _T(" - ");
	if (strcmp(v, inBuf) == 0) {
		cstring += _T("Không cần cập nhật");
		needupdate = FALSE;
	} else if (strcmp(v, inBuf) > 0) {
		cstring += _T("Phiên bản có vấn đề");
		needupdate = TRUE;
	} else {
		 cstring += _T("Cần cập nhật");
		 needupdate = TRUE;
	}
	cstring += _T("(");
	cstring += inBuf;
	cstring += _T(")");
	m_VERSION.SetWindowText(cstring);
}
void AutoDownloadDlg::Update() {
	m_UnpackPath.GetWindowText(cstring);
	if (cstring != "") {
		return;
	}
	char inBuf[MAX_PATH];
	GetPrivateProfileSectionNamesA(inBuf, MAX_PATH + MAX_PATH, FPatchInfo);
	char* bufCHAR;
	bufCHAR = inBuf;
	CString str;
	CString cstring;
	bool cantUpdate = true;
	while (*bufCHAR != 0x00)
	{
		bufCHAR = bufCHAR + strlen(bufCHAR) + 1;
		if (*bufCHAR != 0x00)
		{
			str = bufCHAR;
			CT2A ascii(str);
			GetPrivateProfileStringA(ascii.m_psz, "VersionFrom", NULL, inBuf, MAX_PATH, FPatchInfo);
			if (strcmp(v, inBuf) == 0) {
				cantUpdate = false;
				GetPrivateProfileStringA(ascii.m_psz, "URL_1", NULL, inBuf, MAX_PATH, FPatchInfo);
				DownloadFile(inBuf);
				break;
			}
		}
	}
	if (cantUpdate)
	{
		cstring = _T("Không thể tiến hành update game từ phiên bản hiện tại.");
		m_MESSAGE.SetWindowText(cstring);
	}
}

/*
* List Addon function
*/

UINT Unpack(LPVOID pParam)
{
	AutoDownloadDlg* pDlg = (AutoDownloadDlg*)pParam;

	pDlg->SetProgress1(0);

	CString cstring;
	pDlg->m_FileUrl.GetWindowTextW(cstring);
	CT2A ascii(cstring);
	char *url = ascii.m_psz;
	char *file_name = strrchr(url, '/') + 1;
	cstring = _T("Đang giải nén tệp tin ");
	cstring += file_name;
	pDlg->m_MESSAGE.SetWindowTextW(cstring);

	CString unpack_path;
	pDlg->m_UnpackPath.GetWindowTextW(unpack_path);
	pDlg->SetProgress1(50);
	// Giải nén file .zip
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("Unpack.exe");

	//Command extract
	WCHAR str3[MAX_PATH + 50];
	char cmd[MAX_PATH + MAX_PATH];
	strcpy(cmd, " x ../Patch/");
	strcat(cmd, file_name);
	strcat(cmd, " -o../ -aoa");
	//mbstowcs(wtext, text, strlen(text)+1);
	MultiByteToWideChar(CP_ACP, 0, cmd, -1, str3, 4096);
	ShExecInfo.lpParameters = str3;

	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	cstring = unpack_path;
	DeleteFile(cstring);

	//Reset busy status
	pDlg->SetProgress1(100);
	if(pDlg->m_progress2.GetPos()+5<100) {
		pDlg->SetProgress2(pDlg->m_progress2.GetPos()+5);
	}
	pDlg->m_UnpackPath.SetWindowText(NULL);
	pDlg->SetTimer(100, 200, NULL);

	return TRUE;
}
UINT CURLDownload(LPVOID pParam)
{
	AutoDownloadDlg* pDlg = (AutoDownloadDlg*)pParam;

	CString cstring;

	DWORD dwFlags;

	InternetGetConnectedState(&dwFlags,0);
	CHAR strAgent[64];
	sprintf_s(strAgent,"Agent%ld",timeGetTime());
	HINTERNET hOpen;

	if(!(dwFlags&INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if(!hOpen)
	{
		AfxMessageBox(_T("Internet connected error!"));
		return -1;
	}

	DWORD dwSize;

	VOID* szTemp[16384];
	HINTERNET hConnect;
	CFile file;

	pDlg->m_FileUrl.GetWindowTextW(cstring);
	CT2A ascii(cstring);
	char *url = ascii.m_psz;
	DWORD options = INTERNET_FLAG_NEED_FILE|INTERNET_FLAG_HYPERLINK|INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_RELOAD;


	char *file_name = strrchr(url, '/') + 1;
	cstring = _T("Ðang tải xuống tệp tin ");
	cstring += file_name;
	pDlg->m_MESSAGE.SetWindowTextW(cstring);

	SYSTEMTIME st;
	GetSystemTime(&st);
	LONG time_ms = (st.wYear * 31556926) + (st.wMonth * 2629743) + (st.wDay * 86400) + (st.wHour * 3600) + (st.wMinute * 60) + st.wSecond;
	char strLong[MAX_PATH];
	sprintf(strLong, "%ld", time_ms);
	char new_url[MAX_PATH*MAX_PATH];
	strcat(new_url, url);
	strcat(new_url, "?v=");
	strcat(new_url, strLong);

	if(!(hConnect = InternetOpenUrlA(hOpen, new_url, NULL, NULL, options, 0)))
	{
		AfxMessageBox(_T("Connect Error!"));
		return -1;
	}

	char save_path[MAX_PATH] = "../Patch/";
	strcat(save_path, file_name);
	
	cstring = save_path;

	if (file.Open(cstring, CFile::modeWrite|CFile::modeCreate)==FALSE)
	{
		AfxMessageBox(_T("Can not write data."));
		return -1;
	}

	DWORD dwByteToRead=0;
	DWORD dwSizeOfQequest=4;
	DWORD dwBytes=0;

	if(!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwByteToRead, &dwSizeOfQequest, NULL))
	{
		dwByteToRead = 0;
	}

	DWORD start;
	DWORD end;
	DWORD time;
	time = 0;
	start = timeGetTime();

	CString tempStr;
	char *ext = strrchr( file_name, '.' ) + 1;

	do {
		if (!InternetReadFile(hConnect, szTemp, 16484, &dwSize)) {
			AfxMessageBox(_T("Can not read/write."));
			file.Close();
			return -1;
		}
		if(dwSize==0) {
			break;
		} else {
			file.Write(szTemp,dwSize);
		}
		dwBytes+=dwSize;

		if(dwByteToRead) {
			tempStr.Format(_T("%d%%"),(dwBytes*100)/dwByteToRead);
			int per = (dwBytes*100)/dwByteToRead;
			pDlg->SetProgress1(per);
			pDlg->m_ProgressText.SetWindowTextW(tempStr);
		}
		
		FLOAT fSpeed=0;
		fSpeed=(float)dwBytes;
		end=timeGetTime();
		time=end-start;
		if (time==0) {
			time=10;
		}
		fSpeed /= ((float)time)/1000.0f;
		fSpeed /= 1024.0f;
		tempStr.Format(_T("%.2fKB/s"), fSpeed);
		if (fSpeed > 1024)
		{
			fSpeed /= 1024.0f;
			tempStr.Format(_T("%.2fMB/s"), fSpeed);
		}
		pDlg->m_DownSpeed.SetWindowTextW(tempStr);

	} while (TRUE);

	//Check file EXT
	if (strcmp(ext, "txt") == 0) {
		pDlg->SetProgress2(30);
		//Auto check Version
		pDlg->SetTimer(100, 200, NULL);
	} else {
		if(pDlg->m_progress2.GetPos()+10<100) {
			pDlg->SetProgress2(pDlg->m_progress2.GetPos()+10);
		}
		cstring = save_path;
		pDlg->m_UnpackPath.SetWindowText(cstring);
		AfxBeginThread(Unpack, (LPVOID)pDlg);
	}

	return TRUE;
}
//-----------------------------------------------------------------------------------------
class Launch : public CWinApp
{
public:
	Launch() {  }
public:
	virtual BOOL InitInstance() {
		char inBuf[MAX_PATH];
		AfxEnableControlContainer();
		CWinApp::InitInstance();
		AutoDownloadDlg dlg;
		/*
		GetPrivateProfileStringA("Launch", "PatchInfo", "", inBuf, MAX_PATH, dlg.LaunchINI);
		if (strcmp(inBuf, "") != 0)
		{
			AfxMessageBox(_T("Lỗi file Launch.ini"));
			return FALSE;
		}
		*/
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return TRUE;
	} //close function
};
void AutoDownloadDlg::ShowStartGameBTN(){

	SetProgress2(100);
	m_MESSAGE.SetWindowText(TEXT("Nhấn nút Start Game để bắt đầu."));
	btnBmp.EnableWindow(TRUE);
}
void AutoDownloadDlg::SetProgress1(int to)
{
	m_progress1.SetPos(to);
}
void AutoDownloadDlg::SetProgress2(int to)
{
	if (to <= m_progress2.GetPos())
	{
		return;
	}
	for (int i = m_progress2.GetPos(); i <= to; i++)
	{
		m_progress2.SetPos(i);
	}
}
BOOL AutoDownloadDlg::DownloadFile(char* u)
{
	cstring = u;
	m_FileUrl.SetWindowText(cstring);

	AfxBeginThread(CURLDownload, (LPVOID)this);
	return TRUE;
}
Launch theApp;  //Starts the Application

void AutoDownloadDlg::OnBnClickedCancel() {
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
void AutoDownloadDlg::OnBnClickedMin() {
	this->ShowWindow(SW_MINIMIZE);
}

void AutoDownloadDlg::GoHOME()
{
	CString cstring;
	char inBuf[MAX_PATH];
	GetPrivateProfileStringA("Launch", "HomeURL", "", inBuf, MAX_PATH, LaunchINI);
	cstring = inBuf;
	ShellExecute(NULL, TEXT("Open"), cstring, NULL, NULL, SW_SHOW);
}
void AutoDownloadDlg::GoREG()
{
	CString cstring;
	char inBuf[MAX_PATH];
	GetPrivateProfileStringA("Launch", "RegURL", "", inBuf, MAX_PATH, LaunchINI);
	cstring = inBuf;
	ShellExecute(NULL, TEXT("Open"), cstring, NULL, NULL, SW_SHOW);
}
void AutoDownloadDlg::GoCASH()
{
	CString cstring;
	char inBuf[MAX_PATH];
	GetPrivateProfileStringA("Launch", "ChoiceURL", "", inBuf, MAX_PATH, LaunchINI);
	cstring = inBuf;
	ShellExecute(NULL, TEXT("Open"), cstring, NULL, NULL, SW_SHOW);
}
void AutoDownloadDlg::GoFANPAGE()
{
	CString cstring;
	char inBuf[MAX_PATH];
	GetPrivateProfileStringA("Launch", "Fanpage", "", inBuf, MAX_PATH, LaunchINI);
	cstring = inBuf;
	ShellExecute(NULL, TEXT("Open"), cstring, NULL, NULL, SW_SHOW);
}
void AutoDownloadDlg::GoCONFIG()
{
	ShellExecute(NULL, TEXT("Open"), TEXT("t_Config.exe"), NULL, NULL, SW_SHOW);
}

void AutoDownloadDlg::OnBnClickedStartGame()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, TEXT("Open"), TEXT("Game.exe"), TEXT(" -fl"), NULL, SW_SHOW);
	CDialog::OnOK();
}
