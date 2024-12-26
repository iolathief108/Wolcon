#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <thread>

bool IsScrollLockOn() {
    if ((GetKeyState(VK_SCROLL) & 0x0001) != 0) {
        return true;
    }
    else {
        return false;
    }
}

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

void SetScrollLock(BOOL bState) {
    BYTE keyState[256];

    GetKeyboardState((LPBYTE)&keyState);
    if ((bState && !(keyState[VK_SCROLL] & 1)) ||
        (!bState && (keyState[VK_SCROLL] & 1))) {
        // Simulate a key press
        keybd_event(VK_SCROLL,
                    0x45,
                    KEYEVENTF_EXTENDEDKEY | 0,
                    0);

        // Simulate a key release
        keybd_event(VK_SCROLL,
                    0x45,
                    KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                    0);
    }
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
std::thread* tvThread;
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
    }
    else if (volUp && p.y < 3 && p.x > GetSystemMetrics(SM_CXSCREEN) - 3 && p.x < GetSystemMetrics(SM_CXSCREEN)) {
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
MSLLHOOKSTRUCT* mStruct;

bool topLeftConer;
bool mButtonPress;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        if (wParam == WM_MOUSEWHEEL) {
            mStruct = (MSLLHOOKSTRUCT*)lParam;
            if (wheelHandle((static_cast<int>(mStruct->mouseData) >> 16) > 0))
                return 1;
        }

        if (wParam == WM_MOUSEMOVE) {
            mStruct = (MSLLHOOKSTRUCT*)lParam;
            topLeftConer = static_cast<int>(mStruct->pt.x) <= 0 && static_cast<int>(mStruct->pt.y) <= 0;
        }

        if (wParam == WM_MBUTTONDOWN)
            mButtonPress = true;


        if (wParam == WM_MBUTTONUP) {
            if (mButtonPress && topLeftConer) {
                PostQuitMessage(0);
            }
            mButtonPress = false;
        }
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}


HHOOK kb_hook;
PKBDLLHOOKSTRUCT kp;
DWORD virtualKey;

LRESULT CALLBACK kb_proc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && !IsScrollLockOn()) {
        kp = (PKBDLLHOOKSTRUCT)lParam;

        virtualKey = kp->vkCode;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            if (virtualKey == VK_F6) {
                keyDownUp(VK_MEDIA_PREV_TRACK);
                return 1;
            }
            if (virtualKey == VK_F7) {
                keyDownUp(VK_MEDIA_PLAY_PAUSE);
                return 1;
            }
            if (virtualKey == VK_F8) {
                keyDownUp(VK_MEDIA_NEXT_TRACK);
                return 1;
            }
            if (virtualKey == VK_PRIOR) {
                // goto previous desktop, press win + ctrl + left
                keyDown(VK_LWIN);
                keyDown(VK_LCONTROL);
                keyDown(VK_LEFT);
                keyUp(VK_LEFT);
                keyUp(VK_LCONTROL);
                keyUp(VK_LWIN);
                return 1;
            }
            if (virtualKey == VK_NEXT) {
                // goto next desktop, press win + ctrl + right
                keyDown(VK_LWIN);
                keyDown(VK_LCONTROL);
                keyDown(VK_RIGHT);
                keyUp(VK_RIGHT);
                keyUp(VK_LCONTROL);
                keyUp(VK_LWIN);
                return 1;
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            if (virtualKey == VK_F6) {
                return 1;
            }
            if (virtualKey == VK_F7) {
                return 1;
            }
            if (virtualKey == VK_F8) {
                return 1;
            }
            if (virtualKey == VK_PRIOR) {
                return 1;
            }
            if (virtualKey == VK_NEXT) {
                return 1;
            }
        }
    }

    return CallNextHookEx(kb_hook, nCode, wParam, lParam);
}

#ifdef CONSOLE
int main()
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif
{
    // Initializing
    topLeftConer = false;
    mButtonPress = false;
    taskViewPerformed = false;
    tView = false;

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, 0, 0);
    kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, kb_proc, 0, 0);

    MSG msg{};
    while (GetMessage(&msg, 0, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(mouseHook);
    UnhookWindowsHookEx(kb_hook);
    return 0;
}
