#pragma once
#include <Windows.h>
#include <fstream>
#include <filesystem>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <Shlobj.h>
#include <shellapi.h>

class App
{
public:
	~App();
	static void init();
	static void dispose();
	static App* get();
	static ICoreWebView2Environment* getWebViewEnv();
	static std::wstring getAppPath();
private:
	App();
	void initConfig();
	bool checkRuntime();
	bool checkRegKey(const HKEY& key, const std::wstring& subKey);
	bool ensureAppFolder();
	HRESULT envCallBack(HRESULT result, ICoreWebView2Environment* env);
};

