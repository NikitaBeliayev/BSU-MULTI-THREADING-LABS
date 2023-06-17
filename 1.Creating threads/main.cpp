#include <iostream>
#include <random>
#include <windows.h>
#include "thread"
#include "algorithm"
using namespace std;

struct MyStructure {
private:
    int sleep_time;
    int* array;
    void DeepCopy(int* array) {
        int array_size = _msize(array) / sizeof(int);
        this->array = new int[array_size];
        for (int i = 0; i < array_size; i++) {
            this->array[i] = array[i];
        }
    }
public:
    MyStructure(int sleep_time, int* array) {
        this->sleep_time = sleep_time;
        DeepCopy(array);
    }
    int* GetArray() {
        return this->array;
    }
    int GetSleepTime() {
        return this->sleep_time;
    }
    void SetArray(int* array) {
        DeepCopy(array);
    }
    void SetTime(int time) {
        this->sleep_time = time;
    }
};

int* CreateArray(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1, 100);
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = dist(gen);
    }
    return array;
}


DWORD WINAPI FindMinimum(LPVOID  structure) {
    struct MyStructure* p = (struct MyStructure*)structure;
    int array_size = _msize(p->GetArray()) / sizeof(int);
    int result = 0;
    int time = p->GetSleepTime();
    Sleep(time);
    if (array_size > 0) {
        result = p->GetArray()[0];
    }
    for (int i = 0;i < array_size; i++) {
        if (p->GetArray()[i] < result) {
            result = p->GetArray()[i];
        }
    }
    cout << "Result:"<< result;
    return 0;
}

int main() {
    cout << "Enter the size of array: ";
    int size = 0;
    cin >> size;
    cout << "Enter the time in milliseconds:";
    int time = 0;
    cin >> time;
    int* array = CreateArray(size);
    cout << "Array: ";
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << "\n";
    MyStructure* object = new MyStructure(time, array);

    DWORD workerid;
    HANDLE worker = CreateThread(NULL, 0, &FindMinimum, object, 0, &workerid);
    if (worker == NULL) {
        return GetLastError();
    }
    SuspendThread(worker);
    ResumeThread(worker);
    WaitForSingleObject(worker, INFINITE);
    CloseHandle(worker);
}