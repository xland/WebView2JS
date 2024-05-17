#include "HostImpl.h"

#include "Win.h"
Host::Host(Win* win):win{win}
{
}

STDMETHODIMP Host::HitTest(int htValue)
{
    ReleaseCapture();
    //SetCapture(win->hwnd);
    SendMessage(win->hwnd, WM_NCLBUTTONDOWN, htValue, 0);
    return S_OK;
}

STDMETHODIMP Host::MinimizeWindow()
{
    return S_OK;
}
STDMETHODIMP Host::MaximizeWindow()
{
    return S_OK;
}
STDMETHODIMP Host::CloseWindow()
{
    PostMessage(win->hwnd, WM_CLOSE, 0, 0);
    return S_OK;
}
STDMETHODIMP Host::RestoreWindow()
{
    return S_OK;
}

STDMETHODIMP Host::GetTypeInfoCount(UINT* pctinfo)
{
    *pctinfo = 1;
    return S_OK;
}

STDMETHODIMP Host::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    if (0 != iTInfo)
    {
        return TYPE_E_ELEMENTNOTFOUND;
    }
    if (!m_typeLib)
    {
        RETURN_IF_FAILED(LoadTypeLib(L"WebView2JS.tlb", &m_typeLib));
    }
    return m_typeLib->GetTypeInfoOfGuid(__uuidof(IHost), ppTInfo);
}

STDMETHODIMP Host::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
    wil::com_ptr<ITypeInfo> typeInfo;
    RETURN_IF_FAILED(GetTypeInfo(0, lcid, &typeInfo));
    return typeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

STDMETHODIMP Host::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
    VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
    wil::com_ptr<ITypeInfo> typeInfo;
    RETURN_IF_FAILED(GetTypeInfo(0, lcid, &typeInfo));
    return typeInfo->Invoke(
        this, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}