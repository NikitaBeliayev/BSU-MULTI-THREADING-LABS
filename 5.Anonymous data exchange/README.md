# Lab №3 (Thread Synchronization)
<br>
The main goal of this lab is to learn how to communicate between processes through an anonymous pipe using the WINAPI library.

#### Task description:

1. The Server process should do the following tasks:
    * Enter array size from console (array type: short).
    * Generates array elements.
    * Starts the Client process.
    * Passes an array to the Client process.
    * Gets an array from the Client process.
    * Prints transmitted and received information on the channel to the console.
2. The Client process should do the following tasks:
    * Gets the size of an array of numbers anonymously from the Server process.
    * Receives an array of numbers anonymously from the Server process
    * Requests the number of numbers N and M (N < M ).
    * Determines which of the numbers are in the segment [N,M], passes them through an anonymous channel to the Server process.
    * Prints transmitted and received information on the channel to the console.
    * Finishes work.

#### Сode examples for server process:
<br>
Function to generate an array:

``` cpp
vector<int > GenerateVector(int size){
    vector<int> vec(size);
    srand(time(nullptr));
    for (int i = 0; i < size; i++){ 
       vec[i] = rand() % 10;
    } 
    return vec;
}
```
<br>
Creating an anonymous channel:
<br>
``` cpp
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
```
<br>
Create client process:
<br>
``` cpp
ZeroMemory(&si, sizeof(STARTUPINFO));
CloseHandle(hWritePipe);
si.cb = sizeof(STARTUPINFO);
wsprintf(lpszComLine, "..\\..\\Client\\cmake-build-debug\\Client.exe %d %d",
 (intptr_t) hInheritWritePipe, (intptr_t) hReadPipe);
if (!CreateProcess( NULL,
 lpszComLine,
 NULL,
 NULL,
 TRUE,
 CREATE_NEW_CONSOLE,
 NULL,
 NULL,
 &si,
 &pi)) {
 _cputs("Create process failed.\n");
 _cputs("Press any key to finish.\n");
 _getch();
 return GetLastError();
}
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
_cputs("Press any key to start communication.\n");
_getch();
```
<br>
An example of writing and reading from an anonymous channel, at the end of the example, the result obtained from the client process is displayed:
<br>
``` cpp
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
    if (!ReadFile( hReadPipe,
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
```
<br>
#### Сode examples for client process:
<br>
The client process receives information from the server and creates an array of elements received from the server:

``` cpp
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
    if (!ReadFile( hReadPipe,
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
```
<br>
Running the main job and sending the results to the server process
<br>
``` cpp
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
            cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
    }
}
```