# Lab №2 (Process creation)
<br>
The main goal of this lab is to learn how to synchronize threads using the critical sections and events of the WINAPI library.

### Task description:

##### To complete the task, you must use the following synchronization tools: events, critical sections.

1. The main thread should do the following tasks:
    1. Initialize necessary events and critical sections
    2. Create an array (character type), the dimension and elements of which are entered by the user from the console.
    3. Output the dimension and elements of the original array to the console.
    4. Enter the number k.
    5. Start the Work thread.
    6. Start the CountElement thread.
    7. Recieve a signal from the work thread about the start of summation (use an event).
    8. Display array elements.
    9. Notify the CountElement thread about the start of summing (the start time will occur after all array elements have been displayed, use an event).
    10. Display array elements (total up to k position).
    11. Wait for the CountElement thread signal (use critical section).
    12. Display the result of the CountElement thread.
2. The work thread should do the following tasks:
    1. Ask the user for the time interval required to rest after preparing one element in the array;
    2. Search in the array for elements that do not correspond to numbers or characters of the Latin alphabet (on the left
    put in an array, and fill the remaining elements of the array with spaces). Elements are symbols.
    3. Display elements of the array (final) element by element;
    4. After each finished element, rest for a specified time interval;
    5. Notify the main thread about the beginning of the summation (use an event);
3. The CountElement thread should do the following tasks:
    1. Waits for a message from the main thread about the beginning of the summation (use the event);
    2. Count only the characters corresponding to the punctuation marks of the final array;
    3. Signal the main thread to output the result (use a critical section);
    Creating an array and filling it with elements entered from the keyboard:

``` cpp
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
```
<br>
Creating Events and a Critical Section for Thread Synchronization:
<br>

``` cpp
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
```
<br>
Creating a string to be passed to a child process from the parent process:
<br>

``` cpp
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
```
<br>
The function that the worker thread executes:
<br>

``` cpp
DWORD WINAPI FindSubset(LPVOID mas) {
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
```
<br>
The function that the CountElement thread executes:
<br>

``` cpp
DWORD WINAPI SumElement(LPVOID mas) {
    EnterCriticalSection(&cs);
    DWORD dwWaitResult = WaitForSingleObject(sum_event, INFINITE);
    if (dwWaitResult == WAIT_OBJECT_0) {
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
```
<br>
Synchronizing the work of threads in Main and outputting the results of program execution:
<br>

``` cpp
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
```

<span class="colour" style="color:rgb(204, 120, 50)"></span>