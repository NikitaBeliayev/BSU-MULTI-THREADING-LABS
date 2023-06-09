#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;


vector<int > GenerateVector(int size){
    vector<int> vec(size);
    srand(time(nullptr));
    for (int i = 0; i < size; i++){
        vec[i] = rand() % 10;
    }
    return vec;
}


int main() {
    int size;
    cout << "Enter size of array: ";
    cin >> size;
    vector<int> vec(GenerateVector(size));
    cout << "Source array: ";
    for (int i = 0; i < vec.size(); i++){
        cout << vec[i] << " ";
    }
    cout << endl;

    char lpszComLine[1000];
    HANDLE hEnableRead;
    char lpszEnableRead[] = "ReadEvent";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    HANDLE hWritePipe, hReadPipe, hInheritWritePipe;
    SECURITY_ATTRIBUTES sa;
    hEnableRead = CreateEvent(NULL, FALSE, FALSE, lpszEnableRead);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hReadPipe,&hWritePipe,&sa,0)) {
        _cputs("Create pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    if (!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(),
                         &hInheritWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS))
    {
        _cputs("Duplicate handle failed");
        _cputs("Press any key to finish");
        _getch();
        return GetLastError();
    }
    ZeroMemory(&si, sizeof(STARTUPINFO));
    CloseHandle(hWritePipe);
    si.cb = sizeof(STARTUPINFO);


    wsprintf(lpszComLine, "..\\..\\Client\\cmake-build-debug\\Client.exe %d %d",
             (intptr_t) hInheritWritePipe, (intptr_t) hReadPipe);
    if (!CreateProcess(
            NULL,
            lpszComLine,
            NULL,
            NULL,
            TRUE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &si,
            &pi
    )) {
        _cputs("Create process failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    _cputs("Press any key to start communication.\n");
    _getch();
    DWORD dwBytesWritten;
    WriteFile(hInheritWritePipe, &size ,sizeof(size), &dwBytesWritten ,NULL);

    for (int j = 0; j < vec.size(); j++) {
        if (!WriteFile(
                hInheritWritePipe,
                &vec[j],
                sizeof(vec[j]),
                &dwBytesWritten,
                NULL)) {
            _cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
    }
    WaitForSingleObject(hEnableRead, INFINITE);
    DWORD dwBytesRead;
    int result_size;

    ReadFile(hReadPipe,&result_size,sizeof(result_size),&dwBytesRead,NULL);
    vector<int> result;
    for (int j = 0; j < result_size; j++) {
        int nData;
        if (!ReadFile(
                hReadPipe,
                &nData,
                sizeof(nData),
                &dwBytesRead,
                NULL)) {
            _cputs("Read from the pipe failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        result.push_back(nData);
    }
    cout << "Result: ";
    for (int i = 0; i < result.size(); i++){
        cout << result[i] << " ";
    }
    CloseHandle(hReadPipe);
    CloseHandle(hInheritWritePipe);
    CloseHandle(hEnableRead);
    _cputs("The process finished writing to the pipe.\n");
    _cputs("Press any key to exit.\n");
    _getch();
    return 0;
}
