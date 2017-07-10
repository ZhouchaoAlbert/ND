#ifndef __UIEDITEX_H__
#define __UIEDITEX_H__


#pragma once

namespace DuiLib
{
	class UILIB_API CEditExUI : public  CEditUI
	{
	public:
		CEditExUI();
		~CEditExUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		virtual LRESULT HandleEditWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		void SetBindControl(CControlUI* pControl);
	private:
		CControlUI* m_pBindControl;
	};
}
#endif 