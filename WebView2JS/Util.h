#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <rapidjson/document.h>
std::string convertToUTF8(const std::wstring& wstr);
std::string convertToUTF8(const LPWSTR& wstr);
std::wstring convertToWideChar(const std::string& str);
RECT areaToRect(rapidjson::Value& area,const int& w,const int& h);

