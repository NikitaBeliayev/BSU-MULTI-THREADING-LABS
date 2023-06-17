#include <windows.h>
#include <iostream>
#include <string>
#pragma comment(lib, "kernel32.lib")

using namespace std;

int main(int argc, char *argv[])
{
    HANDLE hMutex;
    HANDLE hEndedEvent;
    cout << "Reader " << atoi(argv[1]) << endl;
    hMutex = OpenMutexW(SYNCHRONIZE, FALSE, L"Mutex");
    if (hMutex == NULL)
    {
        DWORD dwLastError = GetLastError();
        cout << "OpenMutex failed with error " <<  dwLastError << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return dwLastError;
    }

    int nWriterPr = atoi(argv[2]);
    HANDLE* hEvent = new HANDLE[nWriterPr];
    for (int i = 0; i < nWriterPr; i++)
    {
        hEvent[i] = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"Event " + (i + 1));
        if (hEvent[i] == nullptr)
        {
            cout << "Open event failed." << endl;
            cout << "Input any char to exit." << endl;
            cin.get();
            return GetLastError();
        }
    }

    hEndedEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"ReaderEndedEvent " + atoi(argv[1]));
    if (hEvent == nullptr)
    {
        cout << "Open ending event failed." << endl;
        cout << "Input any char to exit." << endl;
        cin.get();
        return GetLastError();
    }

    string buffer;
    for (int i = 0, nMsg = atoi(argv[0]); i < nMsg; i++)
    {
        WaitForSingleObject(hMutex, INFINITE);
        int eventIndex = WaitForMultipleObjects(nWriterPr, hEvent, FALSE, INFINITE) - WAIT_OBJECT_0;
        cout << "Message from Writer " << eventIndex + 1 << "\n";
        ReleaseMutex(hMutex);
    }

    cout << "Reader " << atoi(argv[1]) << " ended" << endl;
    system("pause");

    SetEvent(hEndedEvent);

    for (int i = 0; i < nWriterPr; i++)
    {
        CloseHandle(hEvent[i]);
    }

    delete[] hEvent;
    CloseHandle(hMutex);
    CloseHandle(hEvent);
    CloseHandle(hEndedEvent);
    return 0;
}