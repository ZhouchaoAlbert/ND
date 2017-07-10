#include "stdafx.h"
#include "ImageShowUI.h"

namespace DuiLib
{


CImageShowUI::CImageShowUI():
m_bLoop(FALSE),
m_nImageCount(0),
m_uCurPos(0),
m_uTime(0)
{
}


CImageShowUI::~CImageShowUI()
{
}

LPCTSTR CImageShowUI::GetClass() const
{
	return _T("ImageShowUI");
}


LPVOID  CImageShowUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("ImageShow")) == 0) 
		return static_cast<CImageShowUI*>(this);
	return __super::GetInterface(pstrName);
}

void CImageShowUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("images")) == 0) SetImage(pstrValue);
	else if (_tcscmp(pstrName, _T("imagecount")) == 0) 
		m_nImageCount = _ttoi(pstrValue);
	else if (_tcscmp(pstrName, _T("looptime")) == 0)
		m_uTime = _ttoi(pstrValue);
	else if (_tcscmp(pstrName, _T("loop")) == 0) 
		m_bLoop = _ttoi(pstrValue) > 0 ? TRUE : FALSE;
	else __super::SetAttribute(pstrName, pstrValue);
}

void CImageShowUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
	{
		OnTimer((UINT_PTR)event.wParam);
	}
	__super::DoEvent(event);
}

void CImageShowUI::OnTimer(UINT_PTR idEvent)
{
	if (m_uCurPos < m_nImageCount)
	{
		SetBkImage(m_vecImage[m_uCurPos]);
		m_uCurPos++;
	}
	else
	{
		m_uCurPos = 0;
		SetBkImage(m_vecImage[0]);
	}
}

void CImageShowUI::DoInit()
{
	if (1 == m_nImageCount && 1 == m_vecImage.size())
	{
		SetBkImage(m_vecImage[0]);
	}
	else 
	{
		Start();
	}
}

void CImageShowUI::SetImage(LPCTSTR pstrValue)
{
	CDuiString strValue(pstrValue);
	if (strValue.IsEmpty())
		return;
	CDuiString strT;
	INT32 nIndex = 0;
	while (!strValue.IsEmpty())
	{
		nIndex = strValue.Find(_T(" "));
		if (nIndex == 0)
		{
			strValue = strValue.Right(strValue.GetLength() - 1);
		}
		else if (nIndex > 0)
		{
			strT = strValue.Left(nIndex);
			m_vecImage.push_back(strT);
			strValue = strValue.Right(strValue.GetLength() - nIndex);
		}
		else
		{
			m_vecImage.push_back(strValue);
			strValue.Empty();
		}
	}
}

void CImageShowUI::Start()
{
	if (m_bLoop && m_nImageCount > 0)
	{
		m_pManager->SetTimer(this, 111, 1000);
	}
}

void CImageShowUI::Stop()
{
	m_pManager->KillTimer(this, 111);
}

}