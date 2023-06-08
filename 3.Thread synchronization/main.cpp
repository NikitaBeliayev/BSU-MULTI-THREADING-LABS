#include <iostream>
#include "windows.h"
using namespace std;

HANDLE work_event, sum_event;
char* array;
int count = 0;
CRITICAL_SECTION cs;

DWORD WINAPI FindSubset(LPVOID  mas) {
    int sleep_time = 0;
    cout << "Enter a sleeping time: ";
    cin >> sleep_time;
    int array_size = _msize(array) / sizeof(char);
    int j = 0;
    for (int i = 0; i < array_size; i++){
        if (!isdigit(array[i]) && !isalpha(array[i])){
            swap(array[i], array[j]);
            j++;
        }
    }
    for (int i = 0; i < array_size; i++){
        if (isdigit(array[i]) || isalpha(array[i])){
            array[i] = ' ';
        }
    }
    cout << "Result: ";
    for (int i = 0; i < array_size; i++){
        cout << array[i] << " ";
        Sleep(sleep_time);
    }
    cout << endl;
    SetEvent(work_event);
    return 0;
}

DWORD WINAPI SumElement(LPVOID  mas) {
    EnterCriticalSection(&cs);
    DWORD dwWaitResult = WaitForSingleObject(sum_event, INFINITE);
    if (dwWaitResult == WAIT_OBJECT_0)
    {

        int array_size = _msize(array) / sizeof(char);
        for (int i = 0; i < array_size; i++){
            if (ispunct(array[i])){
                count++;
            }
        }
        LeaveCriticalSection(&cs);
    }
    return 0;
}


int main() {


    int array_size = 0;
    int k;
    cout << "Enter size of array: ";
    cin >> array_size;
    array = new char[array_size];
    for (int i = 0; i < array_size; i++){
        cin >> array[i];
    }
    cout << "Array's size: " << array_size << endl;
    for (int i = 0; i < array_size; i++){
        cout << array[i] << " ";
    }
    cout << endl;
    DWORD workerid;
    InitializeCriticalSection(&cs);
    work_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    sum_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    cout << "Enter k: ";
    cin >> k;
    HANDLE worker = CreateThread(NULL, 0, &FindSubset, NULL, NULL, &workerid);
    DWORD sum_element_id;
    HANDLE sum_element = CreateThread(NULL, 0, &SumElement, NULL, NULL, &sum_element_id);
    if (worker == NULL || sum_element == NULL){
        return GetLastError();
    }
    WaitForSingleObject(work_event, INFINITE);
    cout << "Resulting array from main thread: " << endl;
    for (int i = 0; i < array_size; i++){
        cout << array[i] << " ";
    }
    cout << endl;
    SetEvent(sum_event);
    cout << "Resulting array to an element with index k from main thread: ";
    EnterCriticalSection(&cs);


    cout << endl;
    cout << "Resulting count: " << count;
    LeaveCriticalSection(&cs);
    DeleteCriticalSection(&cs);
    CloseHandle(worker);
    CloseHandle(sum_element);
    CloseHandle(work_event);
    CloseHandle(sum_event);
    return 0;
}
