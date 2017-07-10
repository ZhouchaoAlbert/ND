#pragma once
#include <vector>
using namespace std;
namespace DuiLib{

//images="bg1.png bg2.png" imagecount="2" loop="true" width="570" height="345"

class CImageShowUI : public CControlUI
{
public:
	CImageShowUI();
	~CImageShowUI();
	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual void DoEvent(TEventUI& event);
	void OnTimer(UINT_PTR idEvent);
	virtual void DoInit();
	void SetImage(LPCTSTR pstrValue);
	void Start();
	void Stop();
private:
	CDuiString m_strImages;
	INT32      m_nImageCount;
	BOOL       m_bLoop;
	vector<CDuiString>m_vecImage;
	UINT32    m_uCurPos;
	UINT32    m_uTime;
};

}