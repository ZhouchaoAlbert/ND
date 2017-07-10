#ifndef UIDxAnimation_h__
#define UIDxAnimation_h__

#include <d3d9.h>
#include <math.h>

#pragma once

namespace DuiLib
{
	
typedef enum
{
	UIANIMTYPE_FLAT,
	UIANIMTYPE_SWIPE,
} UITYPE_ANIM;

class CDxAnimationUI;

class UILIB_API CAnimationSpooler
{
public:
	CAnimationSpooler();
	~CAnimationSpooler();

	enum { MAX_BUFFERS = 80 };

	bool Init(HWND hWnd);
	bool PrepareAnimation(HWND hWnd);
	void CancelJobs();
	bool Render();
	
	bool IsAnimating() const;
	bool IsJobScheduled() const;
	bool AddJob(CDxAnimationUI* pJob);

protected:
	void Term();

	COLORREF TranslateColor(LPDIRECT3DSURFACE9 pSurface, COLORREF clr) const;
	bool SetColorKey(LPDIRECT3DTEXTURE9 pTexture, LPDIRECT3DSURFACE9 pSurface, int iTexSize, COLORREF clrColorKey);

	bool PrepareJob_Flat(CDxAnimationUI* pJob);
	bool RenderJob_Flat(const CDxAnimationUI* pJob, LPDIRECT3DSURFACE9 pSurface, DWORD dwTick);

protected:
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;
		FLOAT rhw;
		DWORD color;
		FLOAT tu, tv;
	};
	typedef CUSTOMVERTEX CUSTOMFAN[4];

	HWND m_hWnd;
	bool m_bIsAnimating;
	bool m_bIsInitialized;
	CStdPtrArray m_aJobs;
	D3DFORMAT m_ColorFormat;
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_p3DDevice;
	LPDIRECT3DSURFACE9 m_p3DBackSurface;
	//
	LPDIRECT3DVERTEXBUFFER9 m_p3DVertices[MAX_BUFFERS];
	LPDIRECT3DTEXTURE9 m_p3DTextures[MAX_BUFFERS];
	CUSTOMFAN m_fans[MAX_BUFFERS];
	int m_nBuffers;
};

class UILIB_API CDxAnimationUI
{
public:
	CDxAnimationUI(const CDxAnimationUI& src);
	CDxAnimationUI(UITYPE_ANIM AnimType, DWORD dwStartTick, DWORD dwDuration, COLORREF clrBack, COLORREF clrKey, RECT rcFrom, int xtrans, int ytrans, int ztrans, int alpha, float zrot);
	~CDxAnimationUI(void);
		
	typedef enum
	{
		INTERPOLATE_LINEAR,
		INTERPOLATE_COS,
	} INTERPOLATETYPE;

	typedef struct PLOTMATRIX
	{
		int xtrans;
		int ytrans;
		int ztrans;
		int alpha;
		float zrot;
	} PLOTMATRIX;

	UITYPE_ANIM AnimType;
	DWORD dwStartTick;
	DWORD dwDuration;
	int iBufferStart;
	int iBufferEnd;
	union
	{
		struct 
		{
			COLORREF clrBack;
			COLORREF clrKey;
			RECT rcFrom;
			PLOTMATRIX mFrom;
			INTERPOLATETYPE iInterpolate;
		} plot;
	} data;
};

}

#endif // UIDxAnimation_h__
