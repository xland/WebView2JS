#include "App.h"
#include <rapidjson/document.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "Util.h"
#include <string>
#include <vector>
#include <WebView2EnvironmentOptions.h>
#include "Win.h"




using namespace Microsoft::WRL;

namespace {
	static App* app;
	static rapidjson::Document d;
	static std::vector<Win*> wins;
	static 	std::filesystem::path appPath;
	ICoreWebView2Environment* webViewEnv;
}

App::App()
{
	initConfig();
	if (!checkRuntime()) {
		return;
	}
	if (!ensureAppFolder()) {
		return;
	}
	
	auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
	options->put_AdditionalBrowserArguments(L"--allow-file-access-from-files");
	Microsoft::WRL::ComPtr<ICoreWebView2EnvironmentOptions4> options4;
	HRESULT oeResult = options.As(&options4);
	if (oeResult != S_OK) {
		// UNREACHABLE - cannot continue  todo
	}
	const WCHAR* allowed_origins[1] = {L"*"};
	auto defaultRegistration = Microsoft::WRL::Make<CoreWebView2CustomSchemeRegistration>(L"wv2js");
	defaultRegistration->put_HasAuthorityComponent(TRUE);
	defaultRegistration->put_TreatAsSecure(TRUE);
	defaultRegistration->SetAllowedOrigins(1, allowed_origins);
	ICoreWebView2CustomSchemeRegistration* registrations[1] = { defaultRegistration.Get() };
	options4->SetCustomSchemeRegistrations(1, static_cast<ICoreWebView2CustomSchemeRegistration**>(registrations));


	auto envCBInstance = Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &App::envCallBack);
	HRESULT result = CreateCoreWebView2EnvironmentWithOptions(nullptr, appPath.c_str(), nullptr/*options.Get()*/, envCBInstance.Get());
	if (FAILED(result)) {
		return;
	}
}
App::~App()
{
	for (size_t i = 0; i < wins.size(); i++)
	{
		delete wins[i];
	}
}
void App::init() {
	if (app) {
		return;
	}
	app = new App();
}
App* App::get() {
	return app;
}

ICoreWebView2Environment* App::getWebViewEnv()
{
	return webViewEnv;
}

std::wstring App::getAppPath()
{
	return appPath.wstring();
}

void App::dispose()
{
	delete app;
}
void App::initConfig()
{
	//auto configPath = std::filesystem::canonical(std::filesystem::current_path() / "config.json");
	std::ifstream file("config.json");
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	//auto str = convertToWideChar(content);
	d.Parse(content.c_str());
}

bool App::checkRuntime()
{
	std::wstring regSubKey = L"\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}";
	bool hasRuntime = checkRegKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node" + regSubKey);
	if (hasRuntime) return true;
	hasRuntime = checkRegKey(HKEY_CURRENT_USER, L"Software" + regSubKey);
	if (!hasRuntime) {
		auto result = MessageBox(nullptr, L"error", L"error", MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1);
		if (result == IDOK) {
			ShellExecute(0, 0, L"https://go.microsoft.com/fwlink/p/?LinkId=2124703", 0, 0, SW_SHOW);
		}
		return false;
	}
	return true;
}
bool App::checkRegKey(const HKEY& key, const std::wstring& subKey) {
	size_t bufferSize = 20;
	std::wstring valueBuf;
	valueBuf.resize(bufferSize);
	auto valueSize = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
	auto rc = RegGetValue(key, subKey.c_str(), L"pv", RRF_RT_REG_SZ, nullptr,
		static_cast<void*>(valueBuf.data()), &valueSize
	);
	if (rc == ERROR_SUCCESS)
	{
		valueSize /= sizeof(wchar_t);
		valueBuf.resize(static_cast<size_t>(valueSize - 1));
		if (valueBuf.empty() || valueBuf == L"0.0.0.0") {
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
bool App::ensureAppFolder() {
	
	PWSTR pathTmp;
	auto ret = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &pathTmp);
	if (ret != S_OK) {
		CoTaskMemFree(pathTmp);
		auto result = MessageBox(nullptr, L"error", L"error", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1);
		exit(1);
		return false;
	}
	appPath = pathTmp;
	CoTaskMemFree(pathTmp);
	appPath /= convertToWideChar(d["appName"].GetString());
	if (!std::filesystem::exists(appPath)) {
		auto flag = std::filesystem::create_directory(appPath);
		if (!flag) {
			MessageBox(nullptr, L"error", L"error", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1);
			exit(1);
			return false;
		}
	}
	return true;
}

HRESULT App::envCallBack(HRESULT result, ICoreWebView2Environment* env)
{
	webViewEnv = env;
	rapidjson::Value& winConfigs = d["windows"].GetArray();
	for (size_t i = 0; i < winConfigs.Size(); i++)
	{
		wins.push_back(new Win(winConfigs[i]));
	}
	return S_OK;
}