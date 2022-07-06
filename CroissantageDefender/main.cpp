#include "main.h"

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    std::cout << "Hello World!\n";

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
    wcex.lpszClassName = L"UnvisibleWindowClass";
    wcex.hIconSm = nullptr;

    //Register window class
    if (!RegisterClassEx(&wcex))
        return 0;

    HWND hwnd;
    hwnd = CreateWindowEx(WS_EX_NOACTIVATE, L"UnvisibleWindowClass", L"UnvisibleWindowClass", WS_DISABLED /*WS_OVERLAPPEDWINDOW*/,
        0, 0, 1, 1,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd)
        return 0;

    NOTIFYICONDATAW nid = {};

    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP;
    nid.hIcon = (HICON)LoadImage(NULL, L"C:\\Users\\oginiaux\\Github\\CroissantageDefender\\ico\\croissant-defender.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);

    memcpy(nid.szTip, "MY TOOLTIP\nCA", 128);

    Shell_NotifyIcon(NIM_ADD, &nid);

    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    MSG msg = {};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            
        }
        else
        {
            Sleep(1);
        }
    }

    UnhookWindowsHookEx(hHook);
    Shell_NotifyIcon(NIM_DELETE, &nid);

    return 0;
}

static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    std::cout << "Key pressed!!\n";

    if ((wParam == WM_KEYDOWN || wParam == WM_KEYUP) && lParam != NULL)
    {
        if (((LPKBDLLHOOKSTRUCT)lParam)->vkCode == VK_OEM_MINUS)
        {
            return 1;
        }
    }

    //auto x = CallNextHookEx(NULL, nCode, wParam, lParam);
    return 1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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