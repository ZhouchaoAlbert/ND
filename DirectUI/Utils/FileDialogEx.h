#ifndef __FILE_DIALOG_EX_H__
#define __FILE_DIALOG_EX_H__


#include <commdlg.h>

namespace DuiLib{

struct __POSITION {};
typedef __POSITION* POSITION;

class UILIB_API CFileDialogEx
{
public:
	CFileDialogEx(void);
	~CFileDialogEx(void);

public:
	void SetDefExt(LPCTSTR lpszDefExt);
	void SetFileName(LPCTSTR lpszFileName);
	void SetFlags(DWORD dwFlags);
	void SetFilter(LPCTSTR lpszFilter);
	void SetMultiSel(BOOL bMultiSel = TRUE);
	void SetParentWnd(HWND hParentWnd);
	void SetTitle(LPCTSTR lpszTitle);
	void SetFileNameBufferSize(DWORD dwSize);

	BOOL ShowOpenFileDlg();
	BOOL ShowSaveFileDlg();

	CDuiString GetPathName();
	CDuiString GetFileName();
	CDuiString GetFileExt();
	CDuiString GetFileTitle();
	CDuiString GetFolderPath();

	POSITION GetStartPosition();
	CDuiString GetNextPathName(POSITION& pos);

protected:
	OPENFILENAME m_stOFN;

private:
	TCHAR m_szDefExt[64];
	TCHAR m_szFilter[MAX_PATH];
	UINT32 m_u32FileLen;
	TCHAR * m_lpszFileName;
};

}

#endif