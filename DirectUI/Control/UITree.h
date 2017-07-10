#ifndef __TREE_LAYOUT_H__
#define __TREE_LAYOUT_H__

namespace DuiLib{

#define kTreeDataInterface					_T("TreeData")
#define kTreeLayoutInterface				_T("TreeLayout")
#define kTreeChildElementInterface			_T("TreeChildElement")
#define kTreeElementInterface				_T("TreeElement")

class CTreeLayoutUI;
class CTreeElementUI;

typedef int ( *TREESORTPROC)(CTreeElementUI * pFirst, CTreeElementUI * pSecond, CTreeLayoutUI * pTree);

//数据节点
class UILIB_API CTreeDataUI :public CHorizontalLayoutUI
{
public:
	CTreeDataUI();
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
};

//树子节点
class UILIB_API CTreeChildElementUI : public CVerticalLayoutUI
{
public:
	CTreeChildElementUI();
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	virtual bool GetVisible();
};

//树节点
class UILIB_API CTreeElementUI : public CVerticalLayoutUI
{
public:
	CTreeElementUI();
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
public:
	CTreeDataUI * GetDataElement();
protected:
	int		m_nExpandHeight;
};

//树
class UILIB_API CTreeLayoutUI : public CVerticalLayoutUI
{
public:
	CTreeLayoutUI(void);
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
public:
	LPCTSTR GetItemHotImage();
	void SetItemHotImage(LPCTSTR pStrImage);
	LPCTSTR GetItemSelectedImage();
	void SetItemSelectedImage(LPCTSTR pStrImage);
	void SetItemHotColor(DWORD dwColor);
	DWORD GetItemHotColor() const;
	void SetItemSelectedColor(DWORD dwColor);
	DWORD GetItemSelectedColor() const;
	void DrawItemBk(HDC hDC);
	void DoPaint(HDC hDC, const RECT& rcPaint);
public:
	bool Add(CTreeElementUI* pParent, CTreeElementUI * pControl);
	bool Remove(CTreeElementUI* pControl, bool bDelete);
	void RemoveAll();
	CTreeElementUI * GetRootItem();
	CTreeElementUI * GetNextItem(CTreeElementUI * pElement);
	CTreeElementUI * GetChildItem(CTreeElementUI * pElement);
	void Expand(CTreeElementUI * pElement, UINT nCode);
	bool IsExpand(CTreeElementUI * pElement);
	bool ItemHasChildren(CTreeElementUI * pElement);
	void SelectItem(CTreeElementUI * pItem);
	CTreeElementUI * GetSelectedItem();
	CTreeElementUI * GetParentItem(CTreeElementUI * pElement);
	CTreeElementUI * HitTest(POINT pt);
	void SetItemHeight(CTreeElementUI * pControl, int nHeight);
	RECT GetItemRect(CTreeElementUI * pElement);
	void SortChildren(CContainerUI * pElement, TREESORTPROC pSortProc);
	void SetIndent(int nIndentation);
	int GetIndent();
protected:
	//收缩节点
	void CollapseElement(CTreeElementUI * pElement);
	//展开节点
	void ExpandElement(CTreeElementUI * pElement);
    void QuickSort(CContainerUI * pElement, TREESORTPROC pSortProc, int nLow, int nHigh);
    int Partition(CContainerUI * pElement, TREESORTPROC pSortProc, int nLow, int nHigh);
    void Swap(CContainerUI * pElement, CTreeElementUI * pFirst, CTreeElementUI * pSecond);
protected:
	CTreeElementUI *	m_pSelectedItem;
	CTreeElementUI *	m_pUpHotItem;
	DWORD				m_dwItemHotColor;
	DWORD				m_dwItemSelectedColor;
	CDuiString			m_sItemHotImage;
	CDuiString			m_sItemSelectedImage;
	int					m_nIndentation;//缩进值
};

}

#endif
