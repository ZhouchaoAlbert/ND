#include "SkinApi.h"
#include "SkinEngine.h"
#include <map>
#include <shlobj.h>
#include <stdio.h>
#include <atlconv.h>
#include <string>
#include <DbgHelp.h>
#include <atlstr.h>
#include <vector>
using namespace std;
#include "UIlib.h"
using namespace DuiLib;
#include "UISkinHelper.h"

extern HINSTANCE g_hInstance;
extra_parameters* g_pluginParms;
DuiLib::CSkinEngine* g_pFrame = NULL;
BOOL g_bMSGLoopFlag = TRUE;
std::map<HWND, WNDPROC> g_windowInfoMap;
CDuiString g_tempParam = _T("");
CDuiString g_installPageTabName = _T("");
std::map<CDuiString, CDuiString> g_controlLinkInfoMap;
CDuiString g_skinPath = _T("");

DuiLib::CMessageBox* g_pMessageBox = NULL;

TCHAR g_messageBoxLayoutFileName[MAX_PATH] = {0};
TCHAR g_messageBoxTitleControlName[MAX_PATH] = {0};
TCHAR g_messageBoxTextControlName[MAX_PATH] = {0};

TCHAR g_messageBoxCloseBtnControlName[MAX_PATH] = {0}; 
TCHAR g_messageBoxYESBtnControlName[MAX_PATH] = {0}; 
TCHAR g_messageBoxNOBtnControlName[MAX_PATH] = {0}; 

static UINT_PTR PluginCallback(enum NSPIM msg)
{
	return 0;
}

void InitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	g_pluginParms = extra;
	EXDLL_INIT();
	extra->RegisterPluginCallback(g_hInstance, PluginCallback);
	{	
		TCHAR skinPath[MAX_PATH];
		TCHAR skinLayoutFileName[MAX_PATH];

		ZeroMemory(skinPath, MAX_PATH);
		ZeroMemory(skinLayoutFileName, MAX_PATH);
		
		popstring(skinPath);  // 皮肤路径
		popstring(skinLayoutFileName); //皮肤文件
	
		DuiLib::CPaintManagerUI::SetInstance(g_hInstance);
		DuiLib::CPaintManagerUI::SetResourcePath(skinPath);

		g_skinPath = skinPath;
		g_pFrame = new DuiLib::CSkinEngine();
		if( g_pFrame == NULL ) return;
		g_pFrame->SetSkinXMLPath(skinLayoutFileName);
		g_pFrame->Create(NULL, _T("EC 10.0体验版"), WS_POPUPWINDOW, 0, 0, 0, 0, 0);
		g_pFrame->CenterWindow();
		ShowWindow( g_pFrame->GetHWND(), FALSE);
		pushint( int(g_pFrame->GetHWND()));
	}
}

void ShowPageItem(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR installPageTabName[MAX_PATH];
	TCHAR installPageTabItem[MAX_PATH]; 
	ZeroMemory(installPageTabName, MAX_PATH);
	ZeroMemory(installPageTabItem, MAX_PATH);
	popstring(installPageTabName); // 安装页面tab的名字
	popstring(installPageTabItem); //具体某一页

	if (NULL == g_pFrame)
	{
		MessageBox(NULL, _T("ShowPageItem"), _T("Fail"), MB_OK);
	}
	CTabLayoutUI* pTab = NULL;
	pTab = static_cast<CTabLayoutUI*>(g_pFrame->GetPaintManager().FindControl(installPageTabName));
	if (pTab == NULL)
		return ;
	pTab->SelectItem(_wtoi(installPageTabItem));
}

void FindControlEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);

	popstring(controlName);
	if (NULL  == g_pFrame)
	{
		MessageBox(NULL, _T("FindControl"), _T("Fail"), MB_OK);
	}
	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if( pControl == NULL )
		pushint( - 1 );

	pushint( 0 );
}

void  OnBindControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	popstring(controlName);

	int callbackID = popint();

	if (NULL == g_pFrame)
	{
		MessageBox(NULL, _T("OnControlBindNSISScript"), _T("Fail"), MB_OK);
	}
	g_pFrame->SaveToControlCallbackMap(controlName, callbackID);
}
 
void SetControlFocusEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	popstring(controlName);

	int callbackID = popint();
	if (NULL == g_pFrame)
	{
		MessageBox(NULL, _T("SetControlFocus"), _T("Fail"), MB_OK);
	}
	g_pFrame->SetControlFocus(controlName);
}

void  SetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR controlData[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(controlData, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);

	popstring(controlName);
	popstring(controlData);
	popstring(dataType);

	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if (pControl == NULL)
		return;

	if (_tcsicmp(dataType, _T("enable")) == 0)
	{
		if (_tcsicmp(controlData, _T("true")) == 0)
			pControl->SetEnabled(true);
		else if (_tcsicmp(controlData, _T("false")) == 0)
			pControl->SetEnabled(false);
	}
	else if (_tcsicmp(dataType, _T("text")) == 0)
	{
		if (_tcsicmp(controlData, _T("error")) == 0 || _tcsicmp(controlData, _T("")) == 0)
			pControl->SetText(pControl->GetText());
		else
			pControl->SetText(controlData);
	}
	else if (_tcsicmp(dataType, _T("bkimage")) == 0)
	{
		if (_tcsicmp(controlData, _T("error")) == 0 || _tcsicmp(controlData, _T("")) == 0)
			pControl->SetBkImage(pControl->GetBkImage());
		else
			pControl->SetBkImage(controlData);
	}
	else if (_tcsicmp(dataType, _T("link")) == 0)
	{
		g_controlLinkInfoMap[controlName] = controlData;
	}
	else if (_tcsicmp(dataType, _T("visible")) == 0)
	{
		if (_tcsicmp(controlData, _T("true")) == 0)
			pControl->SetVisible(true);
		else if (_tcsicmp(controlData, _T("false")) == 0)
			pControl->SetVisible(false);
	}
}

void SetButtonData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR controlData[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(controlData, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);

	popstring(controlName);
	popstring(controlData);
	popstring(dataType);

	CButtonUI* pButton = static_cast<CButtonUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if (pButton == NULL)
		return;

	if (_tcsicmp(dataType, _T("enable")) == 0)
	{
		if (_tcsicmp(controlData, _T("true")) == 0)
			pButton->SetEnabled(true);
		else if (_tcsicmp(controlData, _T("false")) == 0)
		{
			pButton->SetEnabled(false);
			pButton->SetDisabledTextColor(0xFFFFFFFF);
			pButton->SetDisabledImage(_T("file='btn_green.png' corner='5,5,5,5' source='0,108,80,144'"));
		}

	}
	else if (_tcsicmp(dataType, _T("bkimage")) == 0)
	{
		if (_tcsicmp(controlData, _T("error")) == 0 || _tcsicmp(controlData, _T("")) == 0)
			pButton->SetBkImage(pButton->GetBkImage());
		else
			pButton->SetBkImage(controlData);
	}
}

void  GetControlData(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR ctlName[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(ctlName, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);
	popstring(ctlName);
	popstring(dataType);

	CControlUI* pControl = static_cast<CControlUI*>(g_pFrame->GetPaintManager().FindControl(ctlName));
	if (pControl == NULL)
		return;

	TCHAR temp[MAX_PATH] = { 0 };
	_tcscpy(temp, pControl->GetText().GetData());
	if (_tcsicmp(dataType, _T("text")) == 0)
		pushstring(temp);
}


void  SetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR ctlName[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(ctlName, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);
	popstring(ctlName);
	popstring(dataType);

	CCheckBoxUI* pControl = static_cast<CCheckBoxUI*>(g_pFrame->GetPaintManager().FindControl(ctlName));
	if (pControl == NULL)
		return;
	if (_tcsicmp(dataType, _T("true")) == 0)
	{
        pControl->Selected(true);
	}
	else
	{
		pControl->Selected(false);
	}
}

void  GetControlCheck(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR ctlName[MAX_PATH];
	TCHAR dataType[MAX_PATH];
	ZeroMemory(ctlName, MAX_PATH);
	ZeroMemory(dataType, MAX_PATH);
	popstring(ctlName);
	popstring(dataType);

	CCheckBoxUI* pControl = static_cast<CCheckBoxUI*>(g_pFrame->GetPaintManager().FindControl(ctlName));
	if (pControl == NULL)
		return;
	bool bCheck = pControl->GetCheck();
	if (bCheck)
	{
		if (_tcsicmp(dataType, _T("Check")) == 0)
			pushint(1);
	}
	else
	{
		if (_tcsicmp(dataType, _T("Check")) == 0)
			pushint(0);
	}
}

void GetDiskSpaceFreeSizeEX(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR PathName[MAX_PATH];
	ZeroMemory(PathName, MAX_PATH);
	popstring(PathName);

	CString strFilePath(PathName);

	vector<CString>vecSplit;
	StringSplit(strFilePath, _T("\\"), vecSplit);
	if (vecSplit.size() > 0)
	{
		//先判断磁盘空间 
		UINT64 u64Size = GetDiskSpaceFreeSize(vecSplit[0]);
		if (u64Size <= 0)
		{
			pushint(0);  //这里说明本地磁盘不存在
		}
		else
		{
			pushint(1);
		}
	}
	else
	{
		pushint(0);  //这里说明本地磁盘不存在
	}
}


void CreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR PathName[MAX_PATH];
	ZeroMemory(PathName, MAX_PATH);
	popstring(PathName);

	BOOL bPath = ValidDirPath(PathName);
	if (bPath)
	{
		pushint(1);
	}
	else
	{
		pushint(0);
	}
}

void GetInstallFileName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR PathName[MAX_PATH];
	ZeroMemory(PathName, MAX_PATH);
	popstring(PathName);

	CString strName =  GetFileName(PathName);

	TCHAR FileName[128];
	memset(FileName, 0, sizeof(FileName));
	_tcscpy(FileName, strName);
	
	pushstring(FileName);
}

void MakeSureCreateDirectoryPath(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR PathName[MAX_PATH];
	ZeroMemory(PathName, MAX_PATH);
	popstring(PathName);
	
	CString strFilePath(PathName);
	strFilePath += _T("\\");
	CStringA strFilePathA;
	W_2_A(strFilePath, strFilePathA);
	BOOL bPath = MakeSureDirectoryPathExists(strFilePathA);
	if (bPath)
	{
		INT32 iStart = strFilePath.Find(_T("\\\\"));
		INT32 iStart2 = strFilePath.Find(_T("/"));
		if (iStart > 0 || iStart2 > 0)
		{
			pushint(0);
		}
		else
		{
			pushint(1);
		}
	}
	else
	{
		pushint(0);
	}

}
void ShowLicense(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(fileName, MAX_PATH);
	popstring( controlName );
	popstring( fileName );
	CDuiString finalFileName = g_skinPath + fileName;	
	CRichEditUI* pRichEditControl = static_cast<CRichEditUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if( pRichEditControl == NULL )
		return;
	// 读许可协议文件，append到richedit中
	USES_CONVERSION;
	FILE* infile;
	infile = fopen(T2A(finalFileName.GetData()), "rb");
	fseek(infile, 0, SEEK_END);
	int nSize = ftell(infile);

	char *pLicense = NULL;
	pLicense = new char[nSize];
	if (pLicense == NULL)
	{
		fclose(infile);
		return;
	}	
	ZeroMemory(pLicense, sizeof(char) * nSize);

	fseek(infile, 0, SEEK_SET);

	int i = fread_s(pLicense, nSize, sizeof(char), nSize, infile);
	pRichEditControl->AppendText(A2T(pLicense));
	if (pLicense != NULL)
	{
		delete[]pLicense;
	}
	fclose(infile);
}


void  SetProgressValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR provalue[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(provalue, MAX_PATH);
	popstring(controlName);
	popstring(provalue);

	CProgressUI* pProgress = static_cast<CProgressUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if (pProgress == NULL)
	{
		//MessageBox(NULL, _T("SetProgressValue"), _T("Fail"), MB_OK);
		ExitProcess(0);
       return ;
	}
	if (_wtoi(provalue) > 1)
	{
		pProgress->SetValue(_wtoi(provalue));
	}
	
}

void  SetPercentValue(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	TCHAR provalue[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	ZeroMemory(provalue, MAX_PATH);
	popstring(controlName);
	popstring(provalue);

	CLabelUI* pPercent = static_cast<CLabelUI*>(g_pFrame->GetPaintManager().FindControl(controlName));
	if (pPercent == NULL)
	{
		//MessageBox(NULL, _T("SetProgressValue"), _T("Fail"), MB_OK);
		ExitProcess(0);
		return;
	}
	if (_wtoi(provalue) > 1)
	{
		CDuiString strPercent;
		strPercent.Format(_T("%s%%"), provalue);
		pPercent->SetText(strPercent);
	}
}


void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	g_pluginParms->ExecuteCodeSegment(idEvent - 1, 0);
}

void  TBCIACreatTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	UINT callback;
	UINT interval;

	callback = popint();
	interval = popint();

	if (!callback || !interval)
		return;

	SetTimer( g_pFrame->GetHWND(), callback, interval, TimerProc );
}

void  TBCIAKillTimer(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	UINT id;
	id = popint();
	KillTimer(g_pFrame->GetHWND(), id);
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED)
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);

	return 0;
}

void SelectFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	BROWSEINFO bi;
	TCHAR result[MAX_PATH];
	TCHAR title[MAX_PATH];
	LPITEMIDLIST resultPIDL;
	ZeroMemory(result, MAX_PATH);
	ZeroMemory(title, MAX_PATH);

	popstring( title );
	bi.hwndOwner = g_pFrame->GetHWND();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = result;
	bi.lpszTitle = title;
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = NULL;
	bi.iImage = 0;

	resultPIDL = SHBrowseForFolder(&bi);
	if (!resultPIDL)
	{
		pushint(-1);
		return;
	}

	if (SHGetPathFromIDList(resultPIDL, result))
	{
		if( result[_tcslen(result)-1] == _T('\\') )
			result[_tcslen(result)-1] = _T('');
		pushstring(result);
	}
	else
		pushint(-1);

	CoTaskMemFree(resultPIDL);
}

void ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	if (NULL == g_pFrame)
	{
		MessageBox(NULL, _T("ShowPage"), _T("Fail"), MB_OK);
	}
	ShowWindow( g_pFrame->GetHWND(), TRUE);

	DuiLib::CPaintManagerUI::MessageLoop();
}

void  ExitSkinEngine(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	ExitProcess( 0 );
}

void  InitMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	popstring(g_messageBoxLayoutFileName);//XML名字
	popstring(g_messageBoxTitleControlName);//标题
	popstring(g_messageBoxTextControlName);//提示内容
	popstring(g_messageBoxCloseBtnControlName);//关闭
	popstring(g_messageBoxYESBtnControlName);//确定
	popstring(g_messageBoxNOBtnControlName);//取消
}

void  ShowMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR TitleName[MAX_PATH];
	TCHAR TextName[MAX_PATH];
	TCHAR YESName[MAX_PATH];
	TCHAR NOName[MAX_PATH];
	ZeroMemory(TitleName, MAX_PATH);
	ZeroMemory(TextName, MAX_PATH);
	popstring(TitleName);//标题
	popstring(TextName);//提示内容
	popstring(YESName);  //确定
	popstring(NOName);   //取消

	if (g_pMessageBox == NULL)
	{
		g_pMessageBox = new DuiLib::CMessageBox();
		if (g_pMessageBox == NULL) return;
		g_pMessageBox->SetSkinXMLPath(g_messageBoxLayoutFileName);
		//g_pMessageBox->Create(g_pFrame->GetHWND(), _T(""), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 0, 0);
		g_pMessageBox->Create(g_pFrame->GetHWND(), _T("安装提示"), UI_WNDSTYLE_FRAME, 0);
		g_pMessageBox->CenterWindow();
	}

	CControlUI* pTitleControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl(g_messageBoxTitleControlName));
	if (pTitleControl != NULL)
	{
		pTitleControl->SetText(TitleName);
	}
	CControlUI* pTipTextControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl(g_messageBoxTextControlName));
	if (pTipTextControl != NULL)
	{
		pTipTextControl->SetText(TextName);
	}
	CControlUI* pYesControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl(g_messageBoxYESBtnControlName));
	if (pYesControl != NULL)
	{
		pYesControl->SetText(YESName);
	}
	CControlUI* pNOControl = static_cast<CControlUI*>(g_pMessageBox->GetPaintManager().FindControl(g_messageBoxNOBtnControlName));
	if (pNOControl != NULL)
	{
		pNOControl->SetText(NOName);
	}
	if (g_pMessageBox->ShowModal() == IDOK)
	{
		pushint(1);
	}
	else
	{
		pushint(2);
	}
}

void  OnBindBoxControl(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	TCHAR controlName[MAX_PATH];
	ZeroMemory(controlName, MAX_PATH);
	popstring(controlName);
	int callbackID = popint();
	if (NULL == g_pMessageBox)
	{
		MessageBox(NULL, _T("OnBindBoxControl"), _T("Fail"), MB_OK);
	}
	g_pMessageBox->SaveToControlCallbackMap(controlName, callbackID);
}

void  CloseMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{


}