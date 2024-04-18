#pragma once
#include "stdafx.h"
#include <functional>
#include <map>
#include <string>
#include <wrl/client.h>
#include "Host_h.h"

class Win;
class Host : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, IHost, IDispatch>
{
public:
    Host(Win* win);
    STDMETHODIMP HitTest(int htValue) override;
    STDMETHODIMP MinimizeWindow() override;
    STDMETHODIMP MaximizeWindow() override;
    STDMETHODIMP CloseWindow() override;
    STDMETHODIMP RestoreWindow() override;
    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override;
    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override;
    STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) override;
    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) override;
private:
    Win* win;
    wil::com_ptr<ITypeLib> m_typeLib;
};

