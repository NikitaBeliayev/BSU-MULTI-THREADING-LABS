#include "iostream"
#include <conio.h>
#include <map>
#include "windows.h"
#include <string>
#include "winuser.h"
using namespace  std;
int main(int argc, char* argv[]) {
    char child_process_name[] = "..\\..\\ChildProcess\\cmake-build-debug\\ChildProcess.exe";
    STARTUPINFO startup_info ;
    PROCESS_INFORMATION process_information ;

    ZeroMemory(&startup_info, sizeof(STARTUPINFO));
    startup_info.cb = sizeof(STARTUPINFO);
    startup_info.dwFlags =STARTF_USEFILLATTRIBUTE;
    startup_info.dwFillAttribute = FOREGROUND_BLUE;
    int size = 0;
    cout << "Enter size of array:  ";
    cin >> size;
    int* array = new int[size];
    for (int i = 0; i < size; i++){
        cout << "Enter data for array[" + to_string(i) + "]: ";
        cin >> array[i];
    }



    int j = 0;
    string temp = "";

    for (int i = 0; i < size; i++){
        temp += std::to_string(array[i]);
        if (i != size - 1){
            temp += " ";
        }
    }
    char* command_line = new char[temp.length() + 1];
    strcpy(command_line, temp.c_str());

    if (CreateProcess(child_process_name, command_line, NULL,
                      NULL, false, CREATE_NEW_CONSOLE,
                      NULL, NULL, &startup_info, &process_information)){
        WaitForSingleObject(process_information.hProcess, INFINITE);
        CloseHandle(process_information.hProcess);
        CloseHandle(process_information.hThread);
    }
    else {
        cout << "Error";
        return 0;
    }
    return 0;
}