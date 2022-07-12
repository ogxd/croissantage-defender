#include "main.h"

#pragma comment(lib, "vfw32.lib")

const wchar_t* WindowTitle = L"CroissantageDefender";
const wchar_t* WindowClass = L"CroissantageDefenderClass";

HWND ghApp = 0;
DWORD g_dwGraphRegister = 0;

IVideoWindow* g_pVW = NULL;
IMediaControl* g_pMC = NULL;
IMediaEventEx* g_pME = NULL;
IGraphBuilder* g_pGraph = NULL;
ICaptureGraphBuilder2* g_pCapture = NULL;
IBaseFilter* pMux = NULL;

/*
void CloseInterfaces(void)
{
    // Stop previewing data
    if (g_pMC)
        g_pMC->StopWhenReady();

    // Relinquish ownership (IMPORTANT!) of the video window.
    // Failing to call put_Owner can lead to assert failures within
    // the video renderer, as it still assumes that it has a valid
    // parent window.
    if (g_pVW)
    {
        g_pVW->put_Visible(OAFALSE);
        g_pVW->put_Owner(NULL);
    }

#ifdef REGISTER_FILTERGRAPH
    // Remove filter graph from the running object table   
    if (g_dwGraphRegister)
        RemoveGraphFromRot(g_dwGraphRegister);
#endif

    // Release DirectShow interfaces
    //SAFE_RELEASE(g_pMC);
    //SAFE_RELEASE(g_pME);
    //SAFE_RELEASE(g_pVW);
    //SAFE_RELEASE(g_pGraph);
    //SAFE_RELEASE(g_pCapture);
}
*/

HRESULT GetInterfaces(void)
{
    HRESULT hr;

    // Create the filter graph
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
        IID_IGraphBuilder, (void**)&g_pGraph);
    if (FAILED(hr))
        return hr;

    // Create the capture graph builder
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC,
        IID_ICaptureGraphBuilder2, (void**)&g_pCapture);
    if (FAILED(hr))
        return hr;

    // Obtain interfaces for media control and Video Window
    hr = g_pGraph->QueryInterface(IID_IMediaControl, (LPVOID*)&g_pMC);
    if (FAILED(hr))
        return hr;

    hr = g_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID*)&g_pVW);
    if (FAILED(hr))
        return hr;

    hr = g_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID*)&g_pME);
    if (FAILED(hr))
        return hr;

    return hr;
}
/*
HRESULT FindCaptureDevice(IBaseFilter** ppSrcFilter)
{
    HRESULT hr = S_OK;
    IBaseFilter* pSrc = NULL;
    IMoniker* pMoniker = NULL;
    ICreateDevEnum* pDevEnum = NULL;
    IEnumMoniker* pClassEnum = NULL;

    if (!ppSrcFilter)
    {
        return E_POINTER;
    }

    // Create the system device enumerator
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
        IID_ICreateDevEnum, (void**)&pDevEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    // Create an enumerator for the video capture devices

    if (SUCCEEDED(hr))
    {
        hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    if (SUCCEEDED(hr))
    {
        // If there are no enumerators for the requested type, then 
        // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
        if (pClassEnum == NULL)
        {
            hr = E_FAIL;
        }
    }

    // Use the first video capture device on the device list.
    // Note that if the Next() call succeeds but there are no monikers,
    // it will return S_FALSE (which is not a failure).  Therefore, we
    // check that the return code is S_OK instead of using SUCCEEDED() macro.

    if (SUCCEEDED(hr))
    {
        hr = pClassEnum->Next(1, &pMoniker, NULL);
        if (hr == S_FALSE)
        {
            return E_FAIL;
        }
    }

    if (SUCCEEDED(hr))
    {
        // Bind Moniker to a filter object
        hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pSrc);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    // Copy the found filter pointer to the output parameter.
    if (SUCCEEDED(hr))
    {
        *ppSrcFilter = pSrc;
        (*ppSrcFilter)->AddRef();
    }

    //SAFE_RELEASE(pSrc);
    //SAFE_RELEASE(pMoniker);
    //SAFE_RELEASE(pDevEnum);
    //SAFE_RELEASE(pClassEnum);

    return hr;
}
*/

HRESULT CaptureVideo()
{
    HRESULT hr;
    IBaseFilter* pSrcFilter = NULL;

    // Get DirectShow interfaces
    hr = GetInterfaces();
    if (FAILED(hr))
    {
        return hr;
    }
    /*
    // Attach the filter graph to the capture graph
    hr = g_pCapture->SetFiltergraph(g_pGraph);
    if (FAILED(hr))
    {
        return hr;
    }

    // Use the system device enumerator and class enumerator to find
    // a video capture/preview device, such as a desktop USB video camera.
    hr = FindCaptureDevice(&pSrcFilter);
    if (FAILED(hr))
    {
        return hr;
    }

    // Add Capture filter to our graph.
    hr = g_pGraph->AddFilter(pSrcFilter, L"Video Capture");
    if (FAILED(hr))
    {
        pSrcFilter->Release();
        return hr;
    }

    hr = g_pCapture->SetOutputFileName( & MEDIASUBTYPE_Avi, L"C:\\Example.avi", & pMux, NULL);

    // Render the preview pin on the video capture filter
    // Use this instead of g_pGraph->RenderFile
    hr = g_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSrcFilter, NULL, pMux);
    if (FAILED(hr))
    {
        pSrcFilter->Release();
        return hr;
    }

    // Now that the filter has been added to the graph and we have
    // rendered its stream, we can release this reference to the filter.
    pSrcFilter->Release();


    // Start previewing video data
    hr = g_pMC->Run();
    if (FAILED(hr))
    {
        return hr;
    }
    */
    return S_OK;
}


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

    CaptureVideo();

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