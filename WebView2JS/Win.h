#pragma once
#include <rapidjson/document.h>
#include <Windows.h>
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
	int nctest(const int& x, const int& y);
	void initCaptionArea();
	rapidjson::Value& config;
	HRGN rgn;
};

