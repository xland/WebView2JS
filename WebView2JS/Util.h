#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
std::string convertToUTF8(const std::wstring& wstr);
std::string convertToUTF8(const LPWSTR& wstr);
std::wstring convertToWideChar(const std::string& str);

