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
    DeleteObject(rgn);
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
    if (!config["frame"].GetBool()) {
        rgn = CreateRectRgn(0, 0, 0, 0);
        initCaptionArea();
        if (config["shadow"].GetBool()) {
            DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
            DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));
            MARGINS margins = { 1,1,1,1 };
            DwmExtendFrameIntoClientArea(hwnd, &margins);
        }
    }
}

int Win::nctest(const int& x, const int& y)
{
    int size{ 6 };
    if (x < size && y < size) {
        return HTTOPLEFT;
    }
    else if (x > size && y < size && x < w - size) {
        return HTTOP;
    }
    else if (y < size && x > w - size) {
        return HTTOPRIGHT;
    }
    else if (y > size && y<h - size && x > w - size) {
        return HTRIGHT;
    }
    else if (y > h - size && x > w - size) {
        return HTBOTTOMRIGHT;
    }
    else if (x > size && y > h - size && x < w - size) {
        return HTBOTTOM;
    }
    else if (x < size && y > h - size) {
        return HTBOTTOMLEFT;
    }
    else if (x < size && y < h - size && y>size) {
        return HTLEFT;
    }
    else if (PtInRegion(rgn, x, y)) {
        return HTCAPTION;
    }
    else {
        return HTCLIENT;
    }
}

void Win::initCaptionArea()
{
    SetRectRgn(rgn, 0, 0, 0, 0);
    rapidjson::Value& areas = config["captionAreas"];
    for (size_t i = 0; i < areas.Size(); i++)
    {
        auto left{ areas[i]["left"].GetInt() };
        auto right{ areas[i]["right"].GetInt() };
        auto top{ areas[i]["top"].GetInt() };
        auto bottom{ areas[i]["bottom"].GetInt() };
        auto width{ areas[i]["width"].GetInt() };
        auto height{ areas[i]["height"].GetInt() };
        if (left < 0) {
            left = right - width;
        }
        if (top < 0) {
            top = bottom - height;
        }
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
        RECT rect{ left,top,right,bottom };
        HRGN hRectRgn = CreateRectRgnIndirect(&rect);
        auto appendType = areas[i]["isAppend"].GetBool() ? RGN_OR: RGN_AND;
        CombineRgn(rgn, rgn, hRectRgn, appendType);
        DeleteObject(hRectRgn);
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
        if (config["frame"].GetBool()) {
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }
        else
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
    }
    case WM_NCHITTEST: {
        if (config["frame"].GetBool()) {
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }
        else {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            ScreenToClient(hWnd, &pt);
            return nctest(pt.x, pt.y);
        }
    }
    case WM_EXITSIZEMOVE: {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        this->x = rect.left;
        this->y = rect.top;
        return true;
    }
    case WM_SIZE: {
        this->w = LOWORD(lParam);
        this->h = HIWORD(lParam);
        return true;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
