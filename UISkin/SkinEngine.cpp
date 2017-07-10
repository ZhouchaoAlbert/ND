#include "StdAfx.h"
#include "SkinEngine.h"
#include "api.h"

extern extra_parameters* g_pluginParms;
extern BOOL g_bMSGLoopFlag;
extern DuiLib::CSkinEngine* g_pFrame; 

extern TCHAR g_messageBoxCloseBtnControlName[MAX_PATH]; 
extern TCHAR g_messageBoxYESBtnControlName[MAX_PATH]; 
extern TCHAR g_messageBoxNOBtnControlName[MAX_PATH]; 
extern DuiLib::CMessageBox* g_pMessageBox;
namespace DuiLib
{
	CDuiString CSkinEngine::GetSkinFolder()
	{
		return _T("");
	}

	CDuiString CSkinEngine::GetSkinFile()
	{
		return m_skinXMLPath;
	}

	LPCTSTR CSkinEngine::GetWindowClassName(void) const
	{
		return _T("SkinEngine");
	}
	UINT CSkinEngine::GetClassStyle() const
	{
		return CS_DBLCLKS;
	}
	UILIB_RESOURCETYPE CSkinEngine::GetResourceType() const
	{
		return UILIB_FILE;
	}

	void CSkinEngine::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete this;
	}

	LRESULT CSkinEngine::ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			CControlUI *pCControlUI = m_PaintManager.GetFocus();
			if (NULL != pCControlUI)
			{
				CDuiString strName = pCControlUI->GetName();
				if (strName == _T("btn_min"))
				{
					return FALSE;
				}
				std::map<CDuiString, int >::iterator iter = m_controlCallbackMap.find(strName);
				if (iter != m_controlCallbackMap.end())
					g_pluginParms->ExecuteCodeSegment(iter->second - 1, 0);

			}
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			return FALSE;
		}
		return FALSE;
	}

	void  CSkinEngine::SetControlFocus(CDuiString ctlName)
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(ctlName));
		if (NULL != pControl)
		{
			m_PaintManager.SetFocusNeeded(pControl);
		}
	}

	void CSkinEngine::Notify(TNotifyUI& msg)
	{
		CDuiString strName  = msg.pSender->GetName();
		std::map<CDuiString, int >::iterator iter = m_controlCallbackMap.find(strName);
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (iter != m_controlCallbackMap.end())
				g_pluginParms->ExecuteCodeSegment(iter->second - 1, 0);
		}
		else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED)
		{
			if( iter != m_controlCallbackMap.end() )
				g_pluginParms->ExecuteCodeSegment( iter->second - 1, 0 );
		}
	}

	void  CSkinEngine::SetSkinXMLPath( LPCTSTR path)
	{
		_tcscpy( m_skinXMLPath, path);
	}

	LPCTSTR CSkinEngine::GetSkinXMLPath()
	{
		return m_skinXMLPath;
	}

	CPaintManagerUI& CSkinEngine::GetPaintManager()
	{
		return m_PaintManager;
	}
	void CSkinEngine::SaveToControlCallbackMap(CDuiString ctlName, int callback)
	{
		m_controlCallbackMap[ctlName] = callback;
	}

	LRESULT CSkinEngine::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_CLOSE)
		{
			bHandled = TRUE;
			return 0;
		}
#if defined(WIN32) && !defined(UNDER_CE)
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if (::IsZoomed(*this) != bZoomed)
		{
			CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // 最大化按钮
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // 还原按钮

			// 切换最大化按钮和还原按钮的状态
			if (pbtnMax && pbtnRestore)
			{
				pbtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
				pbtnRestore->SetVisible(FALSE == bZoomed);
			}
		}
#else
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
		if (SC_RESTORE == (wParam & 0xfff0))
		{
			bHandled = FALSE;
		}
		return lRes;
	}
	//////////////////////////////////////////////////////////////////////////
	///  CMessageBox

	void CMessageBox::Notify(TNotifyUI& msg)
	{
		CDuiString strName = msg.pSender->GetName();
		std::map<CDuiString, int >::iterator iter = m_controlCallbackMap.find(strName);

		if( _tcsicmp( msg.sType, _T("click") ) == 0 )
		{
			if (iter != m_controlCallbackMap.end())
			{
				g_pluginParms->ExecuteCodeSegment( iter->second - 1, 0 );
			}
			else if (strName == g_messageBoxCloseBtnControlName)
			{
				Close(IDCANCEL);
			}
			else if (strName == g_messageBoxYESBtnControlName)
			{
				// PostQuitMessage(0);
				Close(IDOK);
			}
			else if (strName == g_messageBoxNOBtnControlName )
			{
				Close(IDCANCEL);
			}
		}
	}

	void  CMessageBox::SetSkinXMLPath(LPCTSTR path)
	{
		_tcscpy( m_skinXMLPath, path);
	}

	LPCTSTR CMessageBox::GetSkinXMLPath()
	{
		return m_skinXMLPath;
	}

	void CMessageBox::OnFinalMessage(HWND hWnd)
	{
		__super::OnFinalMessage(hWnd);
		delete this;
		g_pMessageBox = NULL;
	}

} // namespace DuiLib 