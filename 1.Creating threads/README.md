# Lab №1 (Thread creation)
<br>
The main purpose of this laboratory work is  learned how create threads using WINAPI library.

### Task description:

1. Create an array of integers, the dimension and elements of which are entered from the console (or generate accidentally).
2. Enter the time to stop and start the worker thread.
3. Create a worker thread, transfer data to the thread: array size, array, etc. Worker thread need to find the minimum element of the array
4. Suspend the worker thread, then restart the thread after a while.
5. Wait for the worker thread to complete.
6. Print the result of the worker thread to the console
7. Shut down

<br>
Array is generated via function called CreateArray, that generates a random array of the integers:
<br>

``` cpp
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
```
<br>
Then the sleep time and array stored in a structure object called Structure which has the following implementation:
<br>

``` cpp
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
```
<br>
Creation of the Worker thread and example of suspending/resuming this thread:
<br>

``` cpp
DWORD workerid;
HANDLE worker = CreateThread(NULL, 0, &FindMinimum, object, 0, &workerid);
if (worker == NULL) { 
    return GetLastError();
}
SuspendThread(worker);
ResumeThread(worker);
WaitForSingleObject(worker, INFINITE);
CloseHandle(worker);
```
<br>
Function for finding the minimum element of the array (It receives object of the strucutre and displays the minimum element ):
<br>

```cpp
DWORD WINAPI FindMinimum(LPVOID structure) {
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
```
<br>
<br>
<span class="colour" style="color:rgb(204, 120, 50)"></span>
