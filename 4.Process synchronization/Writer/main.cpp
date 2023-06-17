#include <windows.h>
#include <iostream>
#include <string>
#pragma comment(lib, "kernel32.lib")

using namespace std;

int main(int argc, char *argv[])
{
    HANDLE hSemaphore;
    HANDLE hEvent;
    HANDLE hEndedEvent;
    cout << "Writer " << atoi(argv[1]) << endl;
    hSemaphore = OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, L"Semaphore");
    if (hSemaphore == NULL)
    {
        DWORD dwLastError = GetLastError();
        cout << "OpenSemaphore failed with error " <<  dwLastError << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return dwLastError;
    }

    hEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"Event " + atoi(argv[1]));
    if (hEvent == nullptr)
    {
        cout << "Open event failed." << endl;
        cout << "Input any char to exit." << endl;
        cin.get();
        return GetLastError();
    }

    hEndedEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"WriterEndedEvent " + atoi(argv[1]));
    if (hEndedEvent == nullptr)
    {
        cout << "Open ending event failed." << endl;
        cout << "Input any char to exit." << endl;
        cin.get();
        return GetLastError();
    }

    string buffer;
    for (int i = 0, nMsg = atoi(argv[0]); i < nMsg; i++)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        cout << "Event " << i + 1 << endl;
        cin >> buffer;
        SetEvent(hEvent);
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }

    SetEvent(hEndedEvent);

    CloseHandle(hSemaphore);
    CloseHandle(hEvent);
    CloseHandle(hEndedEvent);
    return 0;
}