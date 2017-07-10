#ifndef _UIRESOURCDEMGR_H_
#define _UIRESOURCDEMGR_H_

#pragma once

namespace DuiLib 
{
	typedef struct tagTFontInfo
	{
		HFONT hFont;
		CDuiString sFontName;
		int iSize;
		bool bBold;
		bool bUnderline;
		bool bItalic;
		TEXTMETRIC tm;
	} TFontInfo;

	typedef struct tagTImageInfo
	{
		HBITMAP hBitmap;
		int nX;
		int nY;
		bool alphaChannel;
		CDuiString sPath; // 相对路径
		CDuiString sResType;
		DWORD dwMask;
	} TImageInfo;
	

	#define  TheDuiResMgr  UIResourceMgr::GetInstance()

	//此类不能导出只能在dui 内部使用
	class  UIResourceMgr
	{
	protected:
		UIResourceMgr();
		~UIResourceMgr();
	public:
		static UIResourceMgr& GetInstance(); 

		const TImageInfo* GetImage(LPCTSTR bitmap);
		const TImageInfo* GetImageEx(bool& bAddRef, HWND hOwner, LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
		const TImageInfo* AddImage(bool& bAddRef,HWND hOwner, LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
		const TImageInfo* AddImage(bool& bAddRef,HWND hOwner, LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
		bool RemoveImage(HWND hOwner, LPCTSTR bitmap);
		void RemoveAllImages(HWND hOwner);
		void ReloadAllImages();

		HFONT AddFont(bool& bAddRef, LPCTSTR pStrFontName,HWND hOwner, int nSize, bool bBold, bool bUnderline, bool bItalic, HDC hPaintDC);
		HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		TFontInfo* GetFontInfo(HFONT hFont);
		bool FindFont(HFONT hFont);
		bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
		bool RemoveFont(HWND hOwner, HFONT hFont);
	private:
		 CStdStringPtrMap m_mImageHash;
		 CStdStringPtrMap m_mFontHash;

		 std::map<HFONT, CDuiString> m_mFontStrKey;
	};
	
}

#endif