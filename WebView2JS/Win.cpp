#include "Win.h"
#include "Util.h"
#include <format>
#include <windowsx.h>
#include <dwmapi.h>

Win::Win(rapidjson::Value& config):config{config}
{
    initSizeAndPos();
    initWindow();
}

Win::~Win()
{
}

void Win::initSizeAndPos()
{
    w = config["w"].GetInt();
    h = config["h"].GetInt();
    auto centerOfScreen = config["position"]["centerOfScreen"].GetBool();
    if (centerOfScreen) {
        RECT rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
        x = (rect.right - w) / 2;
        y = (rect.bottom - h) / 2;
    }
    else {
        x = config["position"]["x"].GetInt();
        y = config["position"]["y"].GetInt();
    }
}
void Win::initWindow()
{
    auto hinstance = GetModuleHandle(NULL);
    static int num = 0;
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = &Win::RouteWindowMessage;
    wcx.cbWndExtra = sizeof(Win*);
    wcx.hInstance = hinstance;
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    auto clsName = std::format(L"wv2js_{}", num++);
    wcx.lpszClassName = clsName.c_str();
    if (!RegisterClassEx(&wcx))
    {
        MessageBox(NULL, L"注册窗口类失败", L"系统提示", NULL);
        return;
    }
    auto title = convertToWideChar(config["title"].GetString());
    hwnd = CreateWindowEx(NULL, wcx.lpszClassName, title.c_str(), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        x, y, w, h, NULL, NULL, hinstance, static_cast<LPVOID>(this));
    if (!config["frame"].GetBool() && config["shadow"].GetBool()) {
        DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));
        MARGINS margins = { 1,1,1,1 };
        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }
}

LRESULT CALLBACK Win::RouteWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
        LPVOID pThis = pCS->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    auto obj = reinterpret_cast<Win*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (obj) {
        return obj->wndProc(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK Win::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_NCCALCSIZE:
    {
        if (config["frame"].GetBool()) {
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }
        else
        {
            return true;
        }
    }
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
        RECT workArea;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
        mminfo->ptMinTrackSize.x = 1200;
        mminfo->ptMinTrackSize.y = 800;
        mminfo->ptMaxSize.x = workArea.right - workArea.left - 2;
        mminfo->ptMaxSize.y = workArea.bottom - workArea.top - 2;
        mminfo->ptMaxPosition.x = 1;
        mminfo->ptMaxPosition.y = 1;
        return true;
    }
    case WM_NCHITTEST: {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ScreenToClient(hWnd, &pt);
        return HTCLIENT;
    }
    case WM_EXITSIZEMOVE: {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        this->x = rect.left;
        this->y = rect.top;
        return true;
    }
    case WM_SIZE: {
        //onSize(LOWORD(lParam), HIWORD(lParam));
        return true;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
