#include "main.h"

#pragma comment(lib, "vfw32.lib")

const wchar_t* WindowTitle = L"CroissantageDefender";
const wchar_t* WindowClass = L"CroissantageDefenderClass";

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    //Set up window class
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = WS_EX_NOACTIVATE;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = WindowClass;
    wcex.hIconSm = nullptr;

    //Register window class
    if (!RegisterClassEx(&wcex))
        return 0;

    HWND hwnd;
    hwnd = CreateWindowEx(WS_EX_NOACTIVATE, WindowClass, WindowTitle, WS_DISABLED /*WS_OVERLAPPEDWINDOW*/,
        0, 0, 1, 1,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return 0;

    //HWND hWndCap;
    //hWndCap = capCreateCaptureWindow(L"Say Hello", WS_VISIBLE, 0, 0, 640, 480, ::GetDesktopWindow(), 0);

    //if (hWndCap == NULL)
    //    return 0;

    //// connect to the first camera 
    //// for other cameras try index 
    //// 1, 2, in place of the 0 below 
    //if (capDriverConnect(hWndCap, 0))
    //{
    //    capCaptureSequence(hWndCap);
    //    //capCaptureGetSetup()

    //    capGrabFrame(hWndCap);
    //    //capFileSaveAs(hWndCap, L"shot3.bmp");
    //    capFileSaveDIB(hWndCap, L"shot2.bmp");

    //    // the screenshot is in the current 
    //    // directory, usually the same 
    //    // where the exe is created by 
    //    // your compiler 
    //    printf("Saved as shot.bmp!");

    //}

    //DestroyWindow(hWndCap);

    NOTIFYICONDATAW nid = {};

    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP;
    nid.hIcon = (HICON)LoadImage(NULL, L"C:\\Users\\oginiaux\\Github\\CroissantageDefender\\Resources\\icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);

    memcpy(nid.szTip, L"Croissantage Defender", 128);

    Shell_NotifyIcon(NIM_ADD, &nid);

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HHOOK hHookMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);

    MSG msg = {};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // IDLE
        }
        else
        {
            Sleep(1);
        }
    }

    UnhookWindowsHookEx(hHook);
    UnhookWindowsHookEx(hHookMouse);

    Shell_NotifyIcon(NIM_DELETE, &nid);

    return 0;
}

static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if ((wParam == WM_KEYDOWN) && lParam != NULL)
    {
        if (((LPKBDLLHOOKSTRUCT)lParam)->vkCode == VK_NUMPAD0)
        {
            locked = ~locked;
        }
    }

    if (locked)
    {
        return 1;
    }
    else
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (locked)
    {
        return 1;
    }
    else
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}