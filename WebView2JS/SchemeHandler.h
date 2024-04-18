#pragma once
#include <WebView2.h>
#include <wil/resource.h>

// ���������ʵ��ĵط���ʼ����WebView2�����Ϳؼ�
Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_webViewController;
Microsoft::WRL::ComPtr<ICoreWebView2> m_webView;

// ����һ���Զ���Э�鴦�����࣬ʵ��ICoreWebView2CustomSchemeHandler�ӿ�
class MyCustomSchemeHandler final : public Microsoft::WRL::RuntimeClass< Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
    ABI::Microsoft::Web::WebView2::ICoreWebView2CustomSchemeHandler>
{
public:
    STDMETHOD(GetResponseContent)(LPWSTR* content) override
    {
        // ʵ�ַ����Զ�����Ӧ���ݵķ���
        return E_NOTIMPL; // ���ڴ˴����ʵ��ʵ��
    }

    STDMETHOD(GetResponseHeaders)(LPWSTR* headers) override
    {
        // ʵ�ַ����Զ�����Ӧͷ���ķ���
        return E_NOTIMPL; // ���ڴ˴����ʵ��ʵ��
    }

    STDMETHOD(NavigateToStringResult)(HRESULT result) override
    {
        // ����������ķ���
        return S_OK; // ���ڴ˴����ʵ��ʵ��
    }
};