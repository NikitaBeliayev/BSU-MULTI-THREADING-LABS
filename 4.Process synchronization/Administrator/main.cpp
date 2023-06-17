#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#pragma comment(lib, "kernel32.lib")
using namespace std;

bool createReaderProcess(int i, int nReaderMsg,
                         vector<STARTUPINFOW*>& siReader, vector<PROCESS_INFORMATION*>& piReader, int nWriterPr)
{
    wstring temp;
    wchar_t* lpszCommandLine;
    siReader[i] = new STARTUPINFOW;
    ZeroMemory(siReader[i], sizeof(STARTUPINFOW));
    siReader[i]->cb = sizeof(STARTUPINFOW);
    piReader[i] = new PROCESS_INFORMATION;
    temp = to_wstring(nReaderMsg) + L" " + to_wstring(i + 1) + L" " + to_wstring(nWriterPr);
    lpszCommandLine = new wchar_t[temp.length()];
    wcscpy_s(lpszCommandLine, temp.length() + 1, temp.c_str());
    if (!CreateProcessW(L"reader.exe", lpszCommandLine, NULL, NULL, FALSE,
                        CREATE_NEW_CONSOLE, nullptr, NULL, siReader[i], piReader[i]))
    {
        cout << "The new process is not created." << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return false;
    }

    return true;
}

int main()
{
    HANDLE hMutex = NULL;
    hMutex = CreateMutexW(NULL, FALSE, L"Mutex");
    if (hMutex == NULL)
    {
        cout << "Create mutex failed." << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return GetLastError();
    }

    HANDLE hSemaphore = NULL;
    hSemaphore = CreateSemaphoreW(NULL, 3,3,  L"Semaphore");
    if (hSemaphore == NULL)
    {
        cout << "Create semaphore failed." << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return GetLastError();
    }

    int nWriterPr;
    int nReaderPr;
    int nSumPr;
    int nWriterMsg;
    int nReaderMsg;
    int nSumMsg = 0;
    cout << "Enter the number of Writer processes: ";
    cin >> nWriterPr;
    cout << "Enter the number of Reader processes: ";
    cin >> nReaderPr;

    nSumPr = nReaderPr + nWriterPr;
    HANDLE* hEvent = new HANDLE[nWriterPr];
    HANDLE* hEndedEvent = new HANDLE[nSumPr];

    vector<STARTUPINFOW*> siWriter(nWriterPr);
    vector<STARTUPINFOW*> siReader(nReaderPr);
    vector<PROCESS_INFORMATION*> piWriter(nWriterPr);
    vector<PROCESS_INFORMATION*> piReader(nReaderPr);
    wstring temp;
    wchar_t* lpszCommandLine;

    for (int i = 0; i < nWriterPr; i++)
    {
        hEvent[i] = CreateEventW(NULL, FALSE, FALSE, L"Event " + (i + 1));
        if (hEvent[i] == NULL)
        {
            return GetLastError();
        }
        hEndedEvent[i] = CreateEventW(NULL, FALSE, FALSE, L"WriterEndedEvent " + (i + 1));
        if (hEndedEvent[i] == NULL)
        {
            return GetLastError();
        }
        siWriter[i] = new STARTUPINFOW;
        ZeroMemory(siWriter[i], sizeof(STARTUPINFOW));
        siWriter[i]->cb = sizeof(STARTUPINFOW);
        piWriter[i] = new PROCESS_INFORMATION;
        cout << "Enter the number of messages from Writer " << i + 1 << ": ";
        cin >> nWriterMsg;
        nSumMsg += nWriterMsg;
        temp = to_wstring(nWriterMsg) + L" " + to_wstring(i + 1);
        lpszCommandLine = new wchar_t[temp.length()];
        wcscpy_s(lpszCommandLine, temp.length() + 1, temp.c_str());
        if (!CreateProcessW(L"writer.exe", lpszCommandLine, NULL, NULL, FALSE,
                            CREATE_NEW_CONSOLE, nullptr, NULL, siWriter[i], piWriter[i]))
        {
            cout << "The new process is not created." << endl;
            cout << "Press any key to exit." << endl;
            cin.get();
            return GetLastError();
        }
    }

    for (int i = 0; i < nReaderPr - 1; i++)
    {
        hEndedEvent[i + nWriterPr] = CreateEventW(NULL, FALSE, FALSE, L"ReaderEndedEvent " + (i + 1));
        if (hEndedEvent[i + nWriterPr] == NULL)
        {
            return GetLastError();
        }

        if (nSumMsg > 0)
        {
            do
            {
                cout << "Enter the number of received messages for Reader " << i + 1 << " (max: " << nSumMsg << "): ";
                cin >> nReaderMsg;
                if (nReaderMsg <= nSumMsg)
                {
                    break;
                }

                cout << "Incorrect number. Try again\n";
            } while (true);

            nSumMsg -= nReaderMsg;
            if (!createReaderProcess(i, nReaderMsg, siReader, piReader, nWriterPr))
            {
                return GetLastError();
            }

        }
        else
        {
            if (!createReaderProcess(i, 0, siReader, piReader, nWriterPr))
            {
                return GetLastError();
            }
        }

    }

    if (nSumMsg > 0)
    {
        cout << "The number of received messages for Reader " << nReaderPr << ": " << nSumMsg << "\n";
    }

    hEndedEvent[nSumPr - 1] = CreateEventW(NULL, FALSE, FALSE, L"ReaderEndedEvent " + (nReaderPr));
    if (hEndedEvent[nSumPr - 1] == NULL)
    {
        return GetLastError();
    }

    if (!createReaderProcess(nReaderPr - 1, nSumMsg, siReader, piReader, nWriterPr))
    {
        return GetLastError();
    }

    for (int i = 0; i < nSumPr; i++)
    {
        int endedEventIndex = WaitForMultipleObjects(nSumPr, hEndedEvent, FALSE, INFINITE) - WAIT_OBJECT_0;
        if (endedEventIndex < nWriterPr)
        {
            cout << "Writer " << endedEventIndex + 1 << " ended";
        }
        else
        {
            cout << "Reader " << endedEventIndex - nWriterPr + 1 << " ended";
        }

        cout << "\n";
    }

    for (int i = 0; i < nWriterPr; i++)
    {
        WaitForSingleObject(piWriter[i]->hProcess, INFINITE);
        CloseHandle(piWriter[i]->hThread);
        CloseHandle(piWriter[i]->hProcess);
        CloseHandle(hEvent[i]);
        CloseHandle(hEndedEvent[i]);
        delete siWriter[i];
        delete piWriter[i];
    }

    for (int i = 0; i < nReaderPr; i++)
    {
        WaitForSingleObject(piReader[i]->hProcess, INFINITE);
        CloseHandle(piReader[i]->hThread);
        CloseHandle(piReader[i]->hProcess);
        CloseHandle(hEndedEvent[i + nWriterPr]);
        delete siReader[i];
        delete piReader[i];
    }

    delete[] hEvent;
    delete[] hEndedEvent;
    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    return 0;
}

