#ifndef __RICHEDIT_DROP_TARGET_H__
#define __RICHEDIT_DROP_TARGET_H__

#include <textserv.h>
#include <OleIdl.h>
#include <assert.h>

class CRicheditDropTarget : public IDropTarget
{
public:
    CRicheditDropTarget(ITextServices *pTxtSvr)
        :nRef(1)
        , pserv(pTxtSvr)
    {
        assert(pserv);
        pserv->AddRef();
    }

    ~CRicheditDropTarget()
    {
        assert(pserv);
        pserv->Release();
    }

    //IUnkown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
    {
        HRESULT hr = E_NOINTERFACE;
        if (riid == __uuidof(IUnknown))
            *ppvObject = (IUnknown*) this, hr = S_OK;
        else if (riid == __uuidof(IDropTarget))
            *ppvObject = (IDropTarget*)this, hr = S_OK;
        if (SUCCEEDED(hr)) AddRef();
        return hr;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void){ return ++nRef; }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        ULONG uRet = --nRef;
        if (uRet == 0) delete this;
        return uRet;
    }

    //IDropTarget
    virtual HRESULT STDMETHODCALLTYPE DragEnter(
        /* [unique][in] */ IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD *pdwEffect)
    {
        HRESULT hr = S_FALSE;
        IDropTarget *pDropTarget = NULL;
        hr = pserv->TxGetDropTarget(&pDropTarget);
        if (SUCCEEDED(hr))
        {
            hr = pDropTarget->DragEnter(pDataObj, grfKeyState, pt, pdwEffect);
            *pdwEffect = DROPEFFECT_COPY;
            pDropTarget->Release();
        }
        return hr;
    }

    virtual HRESULT STDMETHODCALLTYPE DragOver(
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD *pdwEffect)
    {
        HRESULT hr = S_FALSE;
        IDropTarget *pDropTarget = NULL;
        hr = pserv->TxGetDropTarget(&pDropTarget);
        if (SUCCEEDED(hr))
        {
            hr = pDropTarget->DragOver(grfKeyState, pt, pdwEffect);
            *pdwEffect = DROPEFFECT_COPY;
            pDropTarget->Release();
        }
        return hr;
    }


    virtual HRESULT STDMETHODCALLTYPE DragLeave(void)
    {
        HRESULT hr = S_FALSE;
        IDropTarget *pDropTarget = NULL;
        hr = pserv->TxGetDropTarget(&pDropTarget);
        if (SUCCEEDED(hr))
        {
            hr = pDropTarget->DragLeave();
            pDropTarget->Release();
        }
        return hr;
    }


    virtual HRESULT STDMETHODCALLTYPE Drop(
        /* [unique][in] */ IDataObject *pDataObj,
        /* [in] */ DWORD grfKeyState,
        /* [in] */ POINTL pt,
        /* [out][in] */ DWORD *pdwEffect)
    {
        if (*pdwEffect == DROPEFFECT_NONE) return S_FALSE;
        HRESULT hr = S_FALSE;
        IDropTarget *pDropTarget = NULL;
        hr = pserv->TxGetDropTarget(&pDropTarget);
        if (SUCCEEDED(hr))
        {
            hr = pDropTarget->Drop(pDataObj, grfKeyState, pt, pdwEffect);
            pDropTarget->Release();
        }
        return hr;
    }
protected:
    ITextServices    *pserv;            // pointer to Text Services object
    LONG            nRef;
};
#endif

