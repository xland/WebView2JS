#include "Page.h"
using namespace Microsoft::WRL;
//#include "WindowMain.h"
#include <WebView2.h>


Page::Page(wil::com_ptr<ICoreWebView2> webview, WindowMain* win) :webview{ webview },win{win}
{
	wil::com_ptr<ICoreWebView2Settings> settings;
	webview->get_Settings(&settings);
	settings->put_IsScriptEnabled(TRUE);
	settings->put_AreDefaultScriptDialogsEnabled(TRUE);
	settings->put_IsWebMessageEnabled(TRUE);

	//hostObj = Microsoft::WRL::Make<Host>(win);
	//VARIANT remoteObjectAsVariant = {};
	//hostObj.query_to<IDispatch>(&remoteObjectAsVariant.pdispVal);
	//remoteObjectAsVariant.vt = VT_DISPATCH;
	//webview->AddHostObjectToScript(L"host", &remoteObjectAsVariant);
	//remoteObjectAsVariant.pdispVal->Release();

	Navigate("");
}
Page::~Page()
{

}


BOOL CALLBACK EnumChildProc2(HWND hwndChild, LPARAM lParam) {
	DWORD currentProcessId;
	GetWindowThreadProcessId(hwndChild, &currentProcessId);
	if (currentProcessId == (DWORD)lParam) {
		SetWindowPos(hwndChild, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		auto hwnd2 = GetParent(hwndChild);
		SetWindowPos(hwnd2, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		//TCHAR className[256];
		//GetClassName(pHwnd, className, ARRAYSIZE(className));
		return FALSE;
	}
	return TRUE; // 继续枚举下一个窗口
}


void Page::Navigate(const std::string& url)
{
	// Schedule an async task to navigate to Bing
	auto result = webview->Navigate(L"https://www.baidu.com");
	EventRegistrationToken token;
	auto navigateCB = Callback<ICoreWebView2NavigationStartingEventHandler>(this, &Page::navigationStarting);
	webview->add_NavigationStarting(navigateCB.Get(), &token);
	webview->OpenDevToolsWindow();

	//UINT32 pid;
	//webview->get
	//EnumChildWindows(win->hwnd, EnumChildProc2, pid);

}

HRESULT Page::messageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	//wil::unique_cotaskmem_string message;
	//args->TryGetWebMessageAsString(&message);
	// processMessage(&message);
	//webview->PostWebMessageAsString(message.get());
	return S_OK;
}

HRESULT Page::navigationStarting(ICoreWebView2* webview, ICoreWebView2NavigationStartingEventArgs* args)
{
	//wil::unique_cotaskmem_string uri;
	//args->get_Uri(&uri);
	//std::wstring source(uri.get());
	//if (source.substr(0, 5) != L"https") {
	//	args->put_Cancel(true);
	//}
	return S_OK;
}
void Page::OpenDevTools()
{
	webview->OpenDevToolsWindow();
}
void Page::ExecuteScript()
{
	//webview->AddScriptToExecuteOnDocumentCreated(L"Object.freeze(Object);", nullptr);
	//webview->ExecuteScript(L"window.document.URL;", Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
	//	[](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT {
	//		LPCWSTR URL = resultObjectAsJson;
	//		//doSomethingWithURL(URL);
	//		return S_OK;
	//	}).Get());
}