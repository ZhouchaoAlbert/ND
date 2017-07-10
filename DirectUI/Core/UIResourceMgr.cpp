#include "StdAfx.h"
#include "UIResourceMgr.h"
#include <set>

namespace DuiLib
{

	class CTImageInfoEx
	{
	public:
		CTImageInfoEx(TImageInfo* pData, HWND hOwner);
		~CTImageInfoEx();
	public:
		bool AddRef(HWND hOwner);
		void Release(HWND hOwner);
		BOOL IsEmpty();
		TImageInfo* GetImgInfo();
		int GetRefCout(){return m_nRefCout;}
	private:
		TImageInfo* m_pImgInfo;
		int m_nRefCout;
		std::set<HWND>  m_setOwner;
	};

	class CTFontInfoEx
	{
	public:
		CTFontInfoEx(TFontInfo* pData, HWND hOwner);
		~CTFontInfoEx();
	public:
		bool AddRef(HWND hOwner);
		void Release(HWND hOwner);
		TFontInfo* GetFontInfo();
		HFONT GetHFont();
		BOOL IsEmpty();

		std::set<HWND>  m_setOwner;
	private:
		TFontInfo* m_pFontInfo;
		int m_nRefCout;
		
	};

	UIResourceMgr::UIResourceMgr()
	{

	}

	UIResourceMgr::~UIResourceMgr()
	{
#ifdef  _DEBUG
		ASSERT(m_mImageHash.GetSize() == 0);
		ASSERT(m_mFontHash.GetSize() == 0);

		int nCout = m_mFontHash.GetSize();
		if (nCout != 0)
		{
			TCHAR szName[200] = {};
			for (int i = 0; i < nCout; i++ )
			{
				CDuiString strKey = m_mFontHash.GetAt(i);
				LPVOID lp = m_mFontHash.Find(strKey);
				CTFontInfoEx* pF = (CTFontInfoEx*)lp;

				if(pF && !pF->m_setOwner.empty())
				{
					GetClassName(*(pF->m_setOwner.begin()), szName, 200);
				}
			}
		}
	
		
#endif 	
	}

	const TImageInfo* UIResourceMgr::AddImage(bool& bAddRef, HWND hOwner, LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/)
	{	
		LPVOID lp = m_mImageHash.Find(bitmap);
		TImageInfo* data = NULL;
		bAddRef = false;
		if (lp == NULL)
		{
			if(type != NULL)
			{
				if(isdigit(*bitmap))
				{
					LPTSTR pstr = NULL;
					int iIndex = _tcstol(bitmap, &pstr, 10);
					data = CRenderEngine::LoadImage(iIndex, type, mask);
				}
			}
			else
			{
				data = CRenderEngine::LoadImage(bitmap, NULL, mask);
			}

			if(!data) return NULL;
			if(type != NULL) data->sResType = type;

			CTImageInfoEx* pInfo = new CTImageInfoEx(data, hOwner);
			if(pInfo)
			{
				bAddRef = true;
				if(!m_mImageHash.Insert(bitmap, pInfo))
				{
					pInfo->Release(hOwner);
					delete pInfo;
				}
			}
		}
		else
		{
			//已经存在
			CTImageInfoEx* pInfo  = (CTImageInfoEx*)lp;
			data = pInfo->GetImgInfo();
			bAddRef = pInfo->AddRef(hOwner);
		}
		
		return data;
	}

	const TImageInfo* UIResourceMgr::AddImage(bool& bAddRef,HWND hOwner, LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha)
	{
		if( hBitmap == NULL || iWidth <= 0 || iHeight <= 0 ) return NULL;

		bAddRef = false;
		TImageInfo* data = NULL;
		LPVOID lp = m_mImageHash.Find(bitmap);
		if (lp == NULL)
		{
			data = new TImageInfo;
			data->hBitmap = hBitmap;
			data->nX = iWidth;
			data->nY = iHeight;
			data->alphaChannel = bAlpha;
			//data->sResType = _T("");
			data->dwMask = 0;

			CTImageInfoEx* pInfo = new CTImageInfoEx(data, hOwner);
			if(pInfo)
			{
				bAddRef = true;
				if(!m_mImageHash.Insert(bitmap, pInfo))
				{
					pInfo->Release(hOwner);
					delete pInfo;
				}
			}
		}
		else
		{
			CTImageInfoEx* pInfo = (CTImageInfoEx*)lp;
			data = pInfo->GetImgInfo();
			bAddRef = pInfo->AddRef(hOwner);
		}

		return data;
	}

	const TImageInfo* UIResourceMgr::GetImage(LPCTSTR bitmap)
	{
		CTImageInfoEx* pInfo = static_cast<CTImageInfoEx*>(m_mImageHash.Find(bitmap));

		if (pInfo)
		{
			return pInfo->GetImgInfo();
		}

		return NULL;
	}

	const TImageInfo* UIResourceMgr::GetImageEx(bool& bAddRef, HWND hOwner, LPCTSTR bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/)
	{
		const TImageInfo* data = NULL;
		bAddRef = false;
		CTImageInfoEx* pInfo = static_cast<CTImageInfoEx*>(m_mImageHash.Find(bitmap));
		if (!pInfo)
		{
			data = AddImage(bAddRef,hOwner, bitmap, type, mask);
		}
		else
		{
			data = pInfo->GetImgInfo();
		}

		return data;
	}

	bool UIResourceMgr::RemoveImage(HWND hOwner, LPCTSTR bitmap)
	{
		CDuiString strBitmap = bitmap;

		int iii = m_mImageHash.GetSize();
		CTImageInfoEx* pInfo = static_cast<CTImageInfoEx*>(m_mImageHash.Find(strBitmap.GetData()));
		if (!pInfo) return false;

		if (!pInfo->GetImgInfo())
		{	
			return false;
		}

		pInfo->Release(hOwner);

		if (pInfo->IsEmpty())
		{
			bool bRet = m_mImageHash.Remove(strBitmap.GetData());
			ASSERT(bRet);
			delete pInfo;
			return bRet;
		}

		return false;		
	}

	void UIResourceMgr::RemoveAllImages(HWND hOwner)
	{
		CTImageInfoEx* pInfo = NULL;
		for( int i = 0; i< m_mImageHash.GetSize(); i++ ) 
		{
			 if(LPCTSTR key = m_mImageHash.GetAt(i))
			 {
				 pInfo = static_cast<CTImageInfoEx*>(m_mImageHash.Find(key, false));
				 if(pInfo)
				 {
					// pInfo->Release(hOwner);
					 delete pInfo;
				 }
			 }
		}

		m_mImageHash.RemoveAll();
	}

	void UIResourceMgr::ReloadAllImages()
	{
		bool bRedraw = false;
		TImageInfo* data = NULL;
		TImageInfo* pNewData = NULL;

		for( int i = 0; i< m_mImageHash.GetSize(); i++ )
		{
			if(LPCTSTR bitmap = m_mImageHash.GetAt(i))
			{
				CTImageInfoEx* pInfo = static_cast<CTImageInfoEx*>(m_mImageHash.Find(bitmap));
				if (pInfo != NULL)
				{
					data = pInfo->GetImgInfo();
				}

				if( data != NULL ) 
				{
					if(!data->sResType.IsEmpty())
					{
						if(isdigit(*bitmap))
						{
							LPTSTR pstr = NULL;
							int iIndex = _tcstol(bitmap, &pstr, 10);
							pNewData = CRenderEngine::LoadImage(iIndex, data->sResType.GetData(), data->dwMask);
						}
					}
					else
					{
						  pNewData = CRenderEngine::LoadImage(bitmap, NULL, data->dwMask);
					}

					if( pNewData == NULL ) continue;
					if( data->hBitmap != NULL ) ::DeleteObject(data->hBitmap);
					data->hBitmap = pNewData->hBitmap;
					data->nX = pNewData->nX;
					data->nY = pNewData->nY;
					data->alphaChannel = pNewData->alphaChannel;
					delete pNewData;
				}
			}
		}
	}

	UIResourceMgr& UIResourceMgr::GetInstance()
	{
		static UIResourceMgr s_RoomData;
		return s_RoomData;
	}

	HFONT UIResourceMgr::AddFont(bool& bAddRef, LPCTSTR pStrFontName, HWND hOwner, int nSize, bool bBold, bool bUnderline, bool bItalic, HDC hPaintDC)
	{	
		CDuiString strFontKey;
		strFontKey.Format(_T("%s%d_%d_%d_%d"), pStrFontName, nSize, (int)bBold, (int)bUnderline, (int)bItalic);

		HFONT hFont_ = NULL;
		LPVOID lp = m_mFontHash.Find(strFontKey);
		bAddRef = false;
		if (lp == NULL)
		{
			LOGFONT lf = { 0 };
			::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			_tcsncpy(lf.lfFaceName, pStrFontName, LF_FACESIZE);
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfHeight = -nSize;
			lf.lfQuality = CLEARTYPE_QUALITY;
			if(bBold) lf.lfWeight += FW_BOLD;
			if(bUnderline) lf.lfUnderline = TRUE;
			if(bItalic) lf.lfItalic = TRUE;
			hFont_ = ::CreateFontIndirect(&lf);
			if(hFont_ == NULL) return NULL;

			TFontInfo* pFontInfo = new TFontInfo;
			if(!pFontInfo) return NULL;
			::ZeroMemory(pFontInfo, sizeof(TFontInfo));
			pFontInfo->hFont = hFont_;
			pFontInfo->sFontName = pStrFontName;
			pFontInfo->iSize = nSize;
			pFontInfo->bBold = bBold;
			pFontInfo->bUnderline = bUnderline;
			pFontInfo->bItalic = bItalic;
			if(hPaintDC)
			{
				HFONT hOldFont = (HFONT)::SelectObject(hPaintDC, hFont_);
				::GetTextMetrics(hPaintDC, &pFontInfo->tm);
				::SelectObject(hPaintDC, hOldFont);
			}

			CTFontInfoEx* pInfo = new CTFontInfoEx(pFontInfo, hOwner);
			if(!pInfo) return NULL;
			bAddRef = true;
			if(!m_mFontHash.Insert(strFontKey, pInfo))
			{
				pInfo->Release(hOwner);
				delete pInfo;
				bAddRef = false;
			}

			m_mFontStrKey.insert(std::make_pair(hFont_, strFontKey));
		}
		else 
		{
			CTFontInfoEx* pInfo = (CTFontInfoEx*)lp;
			bAddRef = pInfo->AddRef(hOwner);
			hFont_ = pInfo->GetHFont();
		}

		return hFont_;
	}

	HFONT UIResourceMgr::GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
	{
		CDuiString strFontKey;
		strFontKey.Format(_T("%s%d%d%%d%d"), pStrFontName, nSize, (int)bBold, (int)bUnderline, (int)bItalic);

		LPVOID lp = m_mFontHash.Find(strFontKey);

		if (lp == NULL)
		{
			return NULL;
		}
		else
		{
			CTFontInfoEx* pInfo = (CTFontInfoEx*)lp;
			return pInfo->GetHFont();
		}
	}

	bool UIResourceMgr::FindFont(HFONT hFont)
	{
		std::map<HFONT, CDuiString>::iterator it = m_mFontStrKey.find(hFont);

		if (it != m_mFontStrKey.end())
		{
			LPVOID lp = m_mFontHash.Find(it->second.GetData());
			
			if (lp != NULL)
			{
				return true;
			}
			else
			{
				ASSERT(FALSE);
				return false;
			}	
		}

		return false;
	}

	bool UIResourceMgr::FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
	{
		CDuiString strFontKey;
		strFontKey.Format(_T("%s%d%d%%d%d"), pStrFontName, nSize, (int)bBold, (int)bUnderline, (int)bItalic);

		LPVOID lp = m_mFontHash.Find(strFontKey);
		if (lp != NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool UIResourceMgr::RemoveFont(HWND hOwner, HFONT hFont)
	{
		int ii = m_mFontStrKey.size();
		int iii = m_mFontHash.GetSize();
		if (ii != iii)
		{
			ASSERT(FALSE);
		}
		bool bRet = false;
		std::map<HFONT, CDuiString>::iterator it = m_mFontStrKey.find(hFont);

		if(it != m_mFontStrKey.end())
		{
			CTFontInfoEx* pInfo = static_cast<CTFontInfoEx*>(m_mFontHash.Find(it->second.GetData()));

			if (pInfo != NULL)
			{
				pInfo->Release(hOwner);
			}

			if (pInfo->IsEmpty())
			{
				m_mFontHash.Remove(it->second.GetData());
				delete pInfo;

				m_mFontStrKey.erase(it);
				bRet = true;
			}
		}

		return bRet;
	}

	TFontInfo* UIResourceMgr::GetFontInfo(HFONT hFont)
	{
		std::map<HFONT, CDuiString>::iterator it = m_mFontStrKey.find(hFont);

		if(it != m_mFontStrKey.end())
		{
			CTFontInfoEx* pInfo = static_cast<CTFontInfoEx*>(m_mFontHash.Find(it->second.GetData()));

			if (pInfo != NULL)
			{
				return pInfo->GetFontInfo();
			}
		}

		ASSERT(FALSE);
		return NULL;
	}

	CTImageInfoEx::CTImageInfoEx(TImageInfo* pData, HWND hOwner)
	{
		m_pImgInfo = NULL;
		m_nRefCout = 0;
		if (pData != NULL)
		{
			m_pImgInfo = pData;
			AddRef(hOwner);
		}
	}

	CTImageInfoEx::~CTImageInfoEx()
	{	
		//还有引用的话不应该析构
#ifdef _DEBUG
		if(m_nRefCout != 0) ASSERT(FALSE);
#endif // DEBUG

		if (m_pImgInfo && m_nRefCout == 0)
		{
			CRenderEngine::FreeImage(m_pImgInfo);
			m_pImgInfo = NULL;
		}
	}

	bool CTImageInfoEx::AddRef(HWND hOwner)
	{
		if (m_setOwner.find(hOwner) == m_setOwner.end())
		{
			m_nRefCout++;
			m_setOwner.insert(hOwner);

			return true;
		}

		return false;
	}

	void CTImageInfoEx::Release(HWND hOwner)
	{
		std::set<HWND>::iterator it = m_setOwner.find(hOwner);
		if (it != m_setOwner.end())
		{
			if(--m_nRefCout == 0)
			{
				CRenderEngine::FreeImage(m_pImgInfo);
				m_pImgInfo = NULL;
			}

			m_setOwner.erase(it);
		}
		
#ifdef _DEBUG
		if(m_nRefCout < 0)
		{
			ASSERT(FALSE);
		}
#endif
	}

	TImageInfo* CTImageInfoEx::GetImgInfo()
	{
		return m_pImgInfo;
	}

	BOOL CTImageInfoEx::IsEmpty()
	{
		return (m_nRefCout == 0);
	}


	CTFontInfoEx::CTFontInfoEx(TFontInfo* pData, HWND hOwner)
	{
		m_pFontInfo = NULL;
		m_nRefCout = 0;
		if(pData != NULL)
		{
			m_pFontInfo = pData;
			AddRef(hOwner);
		}
	}

	CTFontInfoEx::~CTFontInfoEx()
	{
		//还有引用的话不应该析构
#ifdef _DEBUG
		if (m_nRefCout != 0 ) ASSERT(FALSE);
#endif // DEBUG
		if(m_pFontInfo && m_nRefCout == 0)
		{
			::DeleteObject(m_pFontInfo->hFont);
			delete m_pFontInfo;
			m_pFontInfo = NULL;
		}
	}

	bool CTFontInfoEx::AddRef(HWND hOwner)
	{
		
		if(m_setOwner.find(hOwner) == m_setOwner.end())
		{
			m_nRefCout++;
			m_setOwner.insert(hOwner);

			return true;
		}

		return false;
	}

	void CTFontInfoEx::Release(HWND hOwner)
	{
		std::set<HWND>::iterator it = m_setOwner.find(hOwner);
		if(it != m_setOwner.end())
		{
			if(--m_nRefCout == 0)
			{
				::DeleteObject(m_pFontInfo->hFont);
				delete m_pFontInfo;
				m_pFontInfo = NULL;
			}

			m_setOwner.erase(it);
		}

#ifdef _DEBUG
		if(m_nRefCout < 0)
		{
			ASSERT(FALSE);
		}
#endif
	}

	TFontInfo* CTFontInfoEx::GetFontInfo()
	{
		return m_pFontInfo;
	}

	HFONT CTFontInfoEx::GetHFont()
	{
		if (m_pFontInfo != NULL)
		{
			return m_pFontInfo->hFont;
		}

		return NULL;
	}

	BOOL CTFontInfoEx::IsEmpty()
	{
		return (m_nRefCout == 0);
	}

}