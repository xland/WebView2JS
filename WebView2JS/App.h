#pragma once
#include <Windows.h>
class App
{
public:
	~App();
	static void init();
	static void dispose();
	static App* get();
	static void webViewEnvReady();
private:
	App();
	void initConfig();
};

