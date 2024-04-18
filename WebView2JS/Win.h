#pragma once
#include <rapidjson/document.h>
#include <Windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <vector>

class Win
{
public:
	Win(rapidjson::Value& config);
	~Win();
	bool createPageController();
	int x, y, w, h;
	HWND hwnd;
private:
	static LRESULT CALLBACK RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void initSizeAndPos();
	void initWindow();
	int nctest(const int& x, const int& y);
	//void initCaptionArea();
	HRESULT pageCtrlCallBack(HRESULT result, ICoreWebView2Controller* controller);
	rapidjson::Value& config;
	std::vector<wil::com_ptr<ICoreWebView2Controller>> ctrls;
	std::vector<wil::com_ptr<ICoreWebView2>> webviews;
	//HRGN rgn;
};

