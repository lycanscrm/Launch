#include "stdafx.h"
#include "UpdateLaunchApp.h"
#include "UpdateLaunch.h"
#include <wininet.h>
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------------------
//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(UpdateLaunchDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------
void UpdateLaunchDlg::OnTimer(UINT_PTR nTimerID)
{
	KillTimer(nTimerID);
	if (nTimerID == 1) {
		SetProgress(20);
		GetPrivateProfileStringA("Launch", "PatchInfo", "", inBuf, MAX_PATH, LaunchINI);
		DownloadFile(inBuf);
	}
	if (nTimerID == 100) {
		if(m_progress1.GetPos() >= 100) {
			m_MESSAGE.SetWindowTextW(_T("Hoàn thành"));
			ShellExecute(NULL, TEXT("Open"), TEXT("AutoDownload.exe"), NULL, NULL, SW_SHOW);
			CDialog::OnOK();
		} else {
			Update();
		}
	}
}
void UpdateLaunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC1, m_MESSAGE);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Control(pDX, IDC_EDIT_URL, m_FileUrl);
	DDX_Control(pDX, IDC_EDIT_UNPACK_PATH, m_UnpackPath);
}

BOOL UpdateLaunchDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}
	/*if (pMsg->message == WM_MOUSEMOVE && (pMsg->wParam & MK_LBUTTON))
	{
		CPoint p = pMsg->pt;
		ScreenToClient(&p);
		CRect r(0, 0, 420, 65);
		if (r.PtInRect(p))
		{
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
			SendMessage(WM_NCLBUTTONUP, HTCAPTION, 0);
			return 1;
		}
	}*/
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL UpdateLaunchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Set app icon
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	SetIcon(hIcon, FALSE);

	this->SetWindowText(TEXT("Update Launch"));
	m_UnpackPath.SetWindowText(NULL);
	SetTimer(1, 0, NULL);

	return true;
}

void UpdateLaunchDlg::SetProgress(int to)
{
	for (int i = m_progress1.GetPos(); i <= to; i++)
	{
		m_progress1.SetPos(i);
	}
}

BOOL UpdateLaunchDlg::DownloadFile(char* u)
{
	cstring = u;
	m_FileUrl.SetWindowText(cstring);

	AfxBeginThread(CURLDownload, (LPVOID)this);
	return TRUE;
}

void UpdateLaunchDlg::Update()
{
	CString cstring;
	m_UnpackPath.GetWindowTextW(cstring);
	if (cstring != "") {
		return;
	}
	char v[MAX_PATH];
	char inBuf[MAX_PATH];
	FILE *f;
	f = fopen(FVersion, "ab+");
	fgets(v, sizeof(v), f);
	fclose(f);
	GetPrivateProfileStringA("Version", "NewLaunch", "", inBuf, MAX_PATH, "../Patch/patchinfo.txt");
	if ((strlen(v) <= 2) || (strcmp(v, inBuf) < 0)) {
		GetPrivateProfileStringA("Launch", "NewLauncher", "", inBuf, MAX_PATH, LaunchINI);

		DownloadFile(inBuf);
	} else {
		SetProgress(100);
		SetTimer(100, 600, NULL);
	}
}

/*
* List Addon function
*/

UINT Unpack(LPVOID pParam)
{
	UpdateLaunchDlg* pDlg = (UpdateLaunchDlg*)pParam;
	CString cstring;
	pDlg->m_UnpackPath.GetWindowTextW(cstring);
	CT2A ascii(cstring);
	char *file_name = strrchr(ascii.m_psz, '/') + 1;
	cstring = _T("Đang giải nén tệp tin ");
	cstring += file_name;
	pDlg->m_MESSAGE.SetWindowTextW(cstring);

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
	strcpy(cmd, " x ");
	strcat(cmd, ascii.m_psz);
	strcat(cmd, " -o../ -aoa");
	//mbstowcs(wtext, text, strlen(text)+1);
	MultiByteToWideChar(CP_ACP, 0, cmd, -1, str3, 4096);
	ShExecInfo.lpParameters = str3;

	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	cstring = ascii.m_psz;
	DeleteFile(cstring);

	//Reset busy status
	pDlg->m_UnpackPath.SetWindowTextW(_T(""));
	pDlg->SetTimer(100, 200, NULL);

	return TRUE;
}
UINT CURLDownload(LPVOID pParam)
{
	UpdateLaunchDlg* pDlg = (UpdateLaunchDlg*)pParam;

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
			if (strcmp(ext, "txt") == 0) {
				if ((pDlg->m_progress1.GetPos()+(per-50))<=60) {
					pDlg->SetProgress(per);
				}
			} else {
				if ((pDlg->m_progress1.GetPos()+(per-50))<=95) {
					pDlg->SetProgress(per);
				}
			}
		}

	} while (TRUE);

	//Check file EXT
	if (strcmp(ext, "txt") == 0) {
		//Auto check Version
		pDlg->SetTimer(100, 200, NULL);
	} else {
		cstring = save_path;
		pDlg->m_UnpackPath.SetWindowText(cstring);
		AfxBeginThread(Unpack, (LPVOID)pDlg);
	}

	return TRUE;
}