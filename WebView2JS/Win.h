#pragma once
#include <rapidjson/document.h>
#include <Windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <vector>
#include "HostImpl.h"

class Win
{
public:
	Win(rapidjson::Value& config);
	~Win();
	int x, y, w, h;
	HWND hwnd;
private:
	static LRESULT CALLBACK RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void initSizeAndPos();
	void initWindow();
	bool createPageController();
	HRESULT pageCtrlCallBack(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT navigationCompleted(ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args);
	HRESULT messageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args);
	rapidjson::Value& config;
	std::vector<wil::com_ptr<ICoreWebView2Controller>> ctrls;
	std::vector<wil::com_ptr<ICoreWebView2>> webviews;
	wil::com_ptr<Host> hostObj;
};

