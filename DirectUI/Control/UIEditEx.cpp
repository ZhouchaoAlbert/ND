#include "StdAfx.h"
#include "UIEditEx.h"

DuiLib::CEditExUI::CEditExUI()
{
	m_bHandleEditWndMsg = true;
	m_pBindControl = NULL;
}

DuiLib::CEditExUI::~CEditExUI()
{
	m_pBindControl = NULL;
}

LPCTSTR DuiLib::CEditExUI::GetClass() const
{
	return _T("EditExUI");
}

LPVOID DuiLib::CEditExUI::GetInterface(LPCTSTR pstrName)
{
	if(_tcscmp(pstrName, _T("EditEx")) == 0) return static_cast<CEditExUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

LRESULT DuiLib::CEditExUI::HandleEditWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	if (uMsg  == WM_KEYDOWN)
	{
		if (wParam == VK_DOWN || wParam == VK_UP /*|| wParam == VK_LEFT || wParam == VK_RIGHT*/)
		{
			if (m_pBindControl != NULL && !m_sText.IsEmpty())
			{
				TEventUI event = { 0 };
				event.Type = UIEVENT_KEYDOWN;
				event.chKey = (TCHAR)wParam;
				if( ::GetKeyState(VK_CONTROL) < 0 ) event.wKeyState |= MK_CONTROL;
				 event.dwTimestamp = ::GetTickCount();
				m_pBindControl->Event(event);

				bHandled = TRUE;
			}
		}
	}
	return 0;
}

void DuiLib::CEditExUI::SetBindControl(CControlUI* pControl)
{
	m_pBindControl = pControl;
}
