#include <windows.h>
#include <iostream>
#include <tchar.h>


void buttonPress(BYTE vkCode)
{
    // Simulate a key press
    keybd_event(vkCode,
                0x45,
                KEYEVENTF_EXTENDEDKEY | 0,
                0);

    // Simulate a key release
    keybd_event(vkCode,
                0x45,
                KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                0);

}

void volumeButton(bool up)
{
    if (up)
        buttonPress(VK_VOLUME_UP);
    else
        buttonPress(VK_VOLUME_DOWN);
}

POINT p{};

bool changeVolume(bool volUp)
{
    GetCursorPos(&p);
    if (p.y > (GetSystemMetrics(SM_CYSCREEN) - 3)) {
        volumeButton(volUp);
        return true;
    }
    return false;
}

HHOOK          mouseHook;
MSLLHOOKSTRUCT *mStruct;

bool corner;
bool mButtonPress;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (nCode == HC_ACTION) {

        if (wParam == WM_MOUSEWHEEL) {
            mStruct = (MSLLHOOKSTRUCT *) lParam;
            if (changeVolume((static_cast<int>(mStruct->mouseData) >> 16) > 0))
                return 1;
        }

        if (wParam == WM_MOUSEMOVE) {
            mStruct = (MSLLHOOKSTRUCT *) lParam;
            corner  = static_cast<int>(mStruct->pt.x) <= 0 && static_cast<int>(mStruct->pt.y) <= 0;
        }

        if (wParam == WM_MBUTTONDOWN)
            mButtonPress = true;


        if (wParam == WM_MBUTTONUP) {
            if (mButtonPress && corner)
                PostQuitMessage(0);
            mButtonPress = false;
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    corner       = false;
    mButtonPress = false;

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, 0, 0);

    MSG msg{};
    while (GetMessage(&msg, 0, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(mouseHook);
    return 0;
}