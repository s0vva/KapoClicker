#include <iostream>
#include <Windows.h>
#include "TlHelp32.h"
using namespace std;

DWORD PID, TID = NULL;
LPVOID rBuffer = NULL;
HMODULE hKernel32 = NULL;
HANDLE hProcess, hThread = NULL;

wchar_t dllPath[MAX_PATH] = L"mcinternal.dll";
size_t dllPathSize = sizeof(dllPath);

string_view processName = "javaw.exe";
std::uintptr_t processId = 0;



int main()
{



    PROCESSENTRY32 entry = { };
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile))
        {
            PID = entry.th32ProcessID;
            break;
        }
    }

    if (snapShot)
        ::CloseHandle(snapShot);
     

    cout << "trying to open handle by this PID\n";
    cout << "PID: ";
    cout << PID << endl;


    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    cout << "got a handle to the process: ";
    cout << hProcess << endl;
    if (hProcess == NULL) {
        cout << "\nSpecified Handle does not exist.";
        // cout << "\n\n" + GetLastError();
        return EXIT_FAILURE;
    }


    rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(dllPath), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
    cout << "allocated ";
    cout << sizeof(dllPath);
    cout << " bytes ";
    cout << " with rwx permisions" << endl;
    if (rBuffer == NULL) {
        MessageBoxW(0, L"Cannot find dll. Please contact the dev.", L"Kapo Client", MB_OK || MB_ICONERROR);
    }


    WriteProcessMemory(hProcess, rBuffer, dllPath, sizeof(dllPath), NULL);
    cout << "wrote: ";
    cout << sizeof(dllPath);
    cout << " bytes ";
    cout << " to process memory" << endl;
    


    hKernel32 = GetModuleHandleW(L"Kernel32");
    cout << "got a handle to Kernel32.dll" << endl;

    LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
    cout << "got address to LoadLibW" << startThis << endl;

    hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hProcess);
    CloseHandle(hThread);
    cout << "injected";

    return 0;

}