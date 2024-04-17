#include "Util.h"

std::string convertToUTF8(const std::wstring& wstr)
{
    const int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}

std::string convertToUTF8(const LPWSTR& wstr)
{
    const int count = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wcslen(wstr), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], count, NULL, NULL);
    return str;
}

std::wstring convertToWideChar(const std::string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], count);
    return wstr;
}

RECT areaToRect(rapidjson::Value& area, const int& w, const int& h)
{
    auto left{ area["left"].GetInt() };
    auto right{ area["right"].GetInt() };
    auto top{ area["top"].GetInt() };
    auto bottom{ area["bottom"].GetInt() };
    auto width{ area["width"].GetInt() };
    auto height{ area["height"].GetInt() };
    if (right < 0) {
        right = left + width;
    }
    else {
        right = w - right;
    }
    if (bottom < 0) {
        bottom = top + height;
    }
    else
    {
        bottom = h - bottom;
    }
    if (left < 0) {
        left = right - width;
    }
    if (top < 0) {
        top = bottom - height;
    }
    return RECT{ left,top,right,bottom };
}
