#pragma once
#include <Windows.h>
class App
{
public:
	~App();
	static void init();
	static void dispose();
	static App* get();
	
private:
	App();
	void initConfig();
	void initWindow();
};

