#![windows_subsystem = "windows"]

use std::mem::zeroed;
use std::ptr;

use winapi::um::wincon::GetConsoleWindow;

use winapi::shared::windef::*;
use winapi::shared::minwindef::*;
use winapi::um::winuser::*;
use winapi::um::shellapi::{Shell_NotifyIconW, NOTIFYICONDATAW, NIM_ADD, NIF_ICON, NIF_TIP};

macro_rules! TEXT {
    ($x:expr) => {{
      use std::ffi::OsStr;
      use std::os::windows::ffi::OsStrExt;
      OsStr::new($x).encode_wide().chain(Some(0)).collect::<Vec<_>>()
    }.as_ptr();}
}

static mut LOCKED: bool = false;

fn main() {
    unsafe {
        setup();
    }
}

unsafe fn setup() {
    let title = TEXT!("CroissantageDefender");
    let class = TEXT!("CroissantageDefenderClass");

    // Retreive process main HWND
    let window = GetConsoleWindow();

    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
    if window != ptr::null_mut() {
        ShowWindow(window, SW_HIDE);
    }

    // Retreive wWinMain HINSTANCE
    let hinstance = winapi::um::libloaderapi::GetModuleHandleW(ptr::null_mut());

    let wcex = WNDCLASSEXW {
        cbSize: std::mem::size_of::<WNDCLASSEXW>() as u32,
        style: WS_EX_NOACTIVATE,
        hInstance: hinstance,
        cbClsExtra: 0,
        cbWndExtra: 0,
        hbrBackground: ptr::null_mut(), // TODO: null_mut or null ?
        hCursor: ptr::null_mut(),
        lpszClassName: class,
        lpszMenuName: ptr::null_mut(),
        hIcon: ptr::null_mut(),
        hIconSm: ptr::null_mut(),
        lpfnWndProc: Some(window_proc),
    };

    let k = winapi::um::winuser::RegisterClassExW(&wcex);
    
    let hwnd = winapi::um::winuser::CreateWindowExW(WS_EX_NOACTIVATE, class, title, WS_DISABLED, 0, 0, 1, 1, ptr::null_mut(), ptr::null_mut(), hinstance, ptr::null_mut());

    if hwnd == (0 as HWND) {
        return;
    }

    winapi::um::winuser::SetWindowsHookExW(WH_MOUSE_LL, Some(low_level_mouse_proc), ptr::null_mut(), 0);
    winapi::um::winuser::SetWindowsHookExW(WH_KEYBOARD_LL, Some(low_level_keyboard_proc), ptr::null_mut(), 0);

    let icon = winapi::um::winuser::LoadImageW(ptr::null_mut(), TEXT!("C:\\Users\\oginiaux\\Github\\CroissantageDefender\\res\\icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);

    let tip = "Croissantage Defender";
    let tip_os: *const u16 = TEXT!(tip);

    let s = std::slice::from_raw_parts(tip_os, tip.len());
    let mut ss = [0; 128];
    ss[..tip.len()].copy_from_slice(s);

    let mut nid = NOTIFYICONDATAW {
        cbSize: std::mem::size_of::<NOTIFYICONDATAW>() as u32,
        hWnd: hwnd,
        uFlags: NIF_ICON | NIF_TIP,
        hIcon: icon as HICON,
        szTip: ss,
        .. zeroed::<NOTIFYICONDATAW>()
    };

    Shell_NotifyIconW(NIM_ADD, &mut nid);

    let mut msg: MSG = MSG { ..zeroed::<MSG>() };

    while (msg.message != WM_QUIT){
        if winapi::um::winuser::PeekMessageW(&mut msg, ptr::null_mut(), 0, 0, PM_REMOVE) == 1 {
            // IDLE
        }
        else
        {
            winapi::um::synchapi::Sleep(1);
        }
    }

}

pub unsafe extern "system" fn window_proc(hwnd: HWND, msg: UINT, wparam: WPARAM, lparam: LPARAM) -> LRESULT {

    return winapi::um::winuser::DefWindowProcW(hwnd, msg, wparam, lparam);
}

pub unsafe extern "system" fn low_level_keyboard_proc(ncode: i32, wparam: WPARAM, lparam: LPARAM) -> LRESULT
{
    if wparam == WM_KEYDOWN as usize && lparam != 0 {
        let lparams = *(lparam as LPKBDLLHOOKSTRUCT);
        if lparams.vkCode as i32 == VK_NUMPAD0 {
            LOCKED = !LOCKED;
        }
    }

    if LOCKED
    {
        return 1;
    }
    else
    {
        return CallNextHookEx(ptr::null_mut(), ncode, wparam, lparam);
    }
}

pub unsafe extern "system" fn low_level_mouse_proc(ncode: i32, wparam: WPARAM, lparam: LPARAM) -> LRESULT
{
    if LOCKED
    {
        return 1;
    }
    else
    {
        return CallNextHookEx(ptr::null_mut(), ncode, wparam, lparam);
    }
}