#pragma  once

#include "UIlib.h"
#include <map>
#include "MsgDef.h"

namespace DuiLib {

	class CSkinEngine : public WindowImplBase
{
public:
	CSkinEngine() {}
	~CSkinEngine() {}

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual UINT GetClassStyle() const;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	void    SetSkinXMLPath( LPCTSTR path );
	LPCTSTR GetSkinXMLPath();

	void SetControlFocus(CDuiString ctlName);
	CPaintManagerUI& GetPaintManager();
	void SaveToControlCallbackMap(CDuiString ctlName, int callback);

private:
	TCHAR                     m_skinXMLPath[MAX_PATH];
	std::map<CDuiString, int> m_controlCallbackMap;
};

//////////////////////////////////////////////////////////////////////////
/// CMessageBox

class CMessageBox : public WindowImplBase
{
public:
	CMessageBox() {}
	~CMessageBox() {}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}
	virtual CDuiString GetSkinFile()
	{
		return m_skinXMLPath;
	}

	virtual LPCTSTR GetWindowClassName(void) const
	{
			return _T("nsTBCIASkinEngine");
	}
	virtual UINT GetClassStyle() const
	{
		return CS_DBLCLKS;
	}
	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_FILE;
	}

	virtual void OnFinalMessage(HWND hWnd);


	void Notify(TNotifyUI& msg);


	void       SetSkinXMLPath( LPCTSTR path );
	LPCTSTR GetSkinXMLPath();


	void SaveToControlCallbackMap( CDuiString ctlName, int callback ) 
	{ 
		m_controlCallbackMap[ctlName] = callback; 
	}
	CPaintManagerUI& GetPaintManager()
	{
		return m_PaintManager;
	}
private:
	TCHAR                     m_skinXMLPath[MAX_PATH];
	std::map<CDuiString, int> m_controlCallbackMap;
};


} 
