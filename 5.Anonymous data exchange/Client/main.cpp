#include <windows.h>
#include <conio.h>
#include<vector>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    HANDLE hWritePipe, hReadPipe;
    HANDLE hEnableRead;
    char lpszEnableRead[] = "ReadEvent";
    hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);
    hWritePipe = (HANDLE) atoi(argv[1]);
    hReadPipe = (HANDLE) atoi(argv[2]);
    DWORD dwBytesRead;
    int size = 0;

    ReadFile(hReadPipe,&size,sizeof(size),&dwBytesRead,NULL);
    vector<int> vec(size);
    for (int j = 0; j < size; j++) {
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
        vec.push_back(nData);
    }

    _cputs("The process finished reading from the pipe.\n");
    int N, M;
    cout << "Enter N: ";
    cin >> N;
    cout << "Enter M: ";
    cin >> M;
    vector<int> result;
    for (int i = 0; i < vec.size(); i++){
        if (vec[i] >= N && vec[i] <= M){
            result.push_back(vec[i]);
        }
    }
    DWORD dwBytesWritten;
    int result_size = result.size();
    WriteFile(hWritePipe, &result_size ,sizeof(result_size), &dwBytesWritten ,NULL);
    for (int j = 0; j < result.size(); j++) {
        if (!WriteFile(
                hWritePipe,
                &result[j],
                sizeof(result[j]),
                &dwBytesWritten,
                NULL)) {
            _cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
    }
    SetEvent(hEnableRead);
    _cputs("The process finished writing to the pipe.\n");
    _cputs("Press any key to finish.\n");
    _getch();
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(hEnableRead);
    return 0;
}