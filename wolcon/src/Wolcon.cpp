#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <thread>

void keyDownUp(BYTE vkCode) {
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

void keyDown(BYTE vkCode) {
    // Simulate a key press
    keybd_event(vkCode,
                0x45,
                KEYEVENTF_EXTENDEDKEY | 0,
                0);
}

void keyUp(BYTE vkCode) {
    // Simulate a key release
    keybd_event(vkCode,
                0x45,
                KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                0);
}

POINT p{};

bool taskViewPerformed;
unsigned short count;
std::thread *tvThread;
bool tView;

void taskViewRunner() {
    tView = true;
    for (count = 0; count < 335; ++count) {
        Sleep(1);
    }
    taskViewPerformed = false;
}

bool wheelHandle(bool volUp) {
    GetCursorPos(&p);
    if (p.y > (GetSystemMetrics(SM_CYSCREEN) - 3)) {
        if (volUp)
            keyDownUp(VK_VOLUME_UP);
        else
            keyDownUp(VK_VOLUME_DOWN);
        return true;
    } else if (volUp && p.y < 3 && p.x > GetSystemMetrics(SM_CXSCREEN) - 3 && p.x < GetSystemMetrics(SM_CXSCREEN)) {
        count = 0;
        if (!taskViewPerformed) {
            keybd_event(VK_LWIN,
                        0x45,
                        KEYEVENTF_EXTENDEDKEY | 0,
                        0);
            keybd_event(VK_TAB,
                        0x45,
                        KEYEVENTF_EXTENDEDKEY | 0,
                        0);

            keybd_event(VK_LWIN,
                        0x45,
                        KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                        0);
            keybd_event(VK_TAB,
                        0x45,
                        KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                        0);

            taskViewPerformed = true;

            if (tView) {
                tvThread->detach();
                delete tvThread;
            }
            tvThread = new std::thread(taskViewRunner);
        }
    }
    return false;
}

HHOOK mouseHook;
MSLLHOOKSTRUCT *mStruct;

bool topLeftConer;
bool mButtonPress;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION) {

        if (wParam == WM_MOUSEWHEEL) {
            mStruct = (MSLLHOOKSTRUCT *) lParam;
            if (wheelHandle((static_cast<int>(mStruct->mouseData) >> 16) > 0))
                return 1;
        }

        if (wParam == WM_MOUSEMOVE) {
            mStruct = (MSLLHOOKSTRUCT *) lParam;
            topLeftConer = static_cast<int>(mStruct->pt.x) <= 0 && static_cast<int>(mStruct->pt.y) <= 0;
        }

        if (wParam == WM_MBUTTONDOWN)
            mButtonPress = true;


        if (wParam == WM_MBUTTONUP) {
            if (mButtonPress && topLeftConer)
                PostQuitMessage(0);
            mButtonPress = false;
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

#ifdef CONSOLE
int main() {
#else
int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#endif
    topLeftConer = false;
    mButtonPress = false;
    taskViewPerformed = false;
    tView = false;

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, 0, 0);

    MSG msg{};
    while (GetMessage(&msg, 0, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(mouseHook);
    return 0;
}
