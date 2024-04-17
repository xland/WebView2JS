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
#include "Win.h"

namespace {
	static App* app;
	static rapidjson::Document d;
	static std::vector<Win*> wins;
}

App::App()
{
	initConfig();
	initWindow();
}
void App::initConfig()
{
	//auto configPath = std::filesystem::canonical(std::filesystem::current_path() / "config.json");
	std::ifstream file("config.json");
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	//auto str = convertToWideChar(content);
	d.Parse(content.c_str());
}
void App::initWindow()
{
	rapidjson::Value& winConfigs = d["windows"].GetArray();
	for (size_t i = 0; i < winConfigs.Size(); i++)
	{
		wins.push_back(new Win(winConfigs[i]));
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
void App::dispose()
{
	delete app;
}