#include <iostream>
#include <Windows.h>

bool BlockHit = false;



void RightClick() {
    POINT cursorpos;
    GetCursorPos(&cursorpos);
    SetCursorPos(cursorpos.x, cursorpos.y);
    mouse_event(MOUSEEVENTF_RIGHTDOWN, cursorpos.x, cursorpos.y, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, cursorpos.x, cursorpos.y, 0, 0);
    Sleep(25);
}

void LeftClick() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    SetCursorPos(cursorPos.x, cursorPos.y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, cursorPos.x, cursorPos.y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, cursorPos.x, cursorPos.y, 0, 0);
    Sleep(60);
}


void LeftBlockHit() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    SetCursorPos(cursorPos.x, cursorPos.y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, cursorPos.x, cursorPos.y, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTDOWN, cursorPos.x, cursorPos.y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, cursorPos.x, cursorPos.y, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, cursorPos.x, cursorPos.y, 0, 0);
    Sleep(60);
}






DWORD WINAPI MainThread(LPVOID param) {
    while (true) {
        if (GetAsyncKeyState(VK_XBUTTON2)) {
            if (BlockHit) {
                LeftBlockHit();
            }
            else {
                LeftClick();
            }

        }
        else if (GetAsyncKeyState(VK_XBUTTON1)) {
            RightClick();
        }
        else if (GetAsyncKeyState(VK_HOME)) {
            Sleep(100);
            MessageBoxW(NULL, L"Kapo was dissconnected", L"Kapo Client", MB_OK || MB_ICONWARNING);
            break;
        }
        else if (GetAsyncKeyState(0x4B)) {
            if (BlockHit == false) {
                BlockHit = true;
                MessageBoxW(NULL, L"Blockhit enabled", L"Kapo Client", MB_OK || MB_ICONWARNING);
            }
            else if (BlockHit) {
                BlockHit = false;
                MessageBoxW(NULL, L"Blockhit disabled", L"KapoClient", MB_OK || MB_ICONWARNING);
            }


        }
    }
    return EXIT_SUCCESS;
}


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    if (fdwReason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, MainThread, hinstDLL, 0, 0);
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}