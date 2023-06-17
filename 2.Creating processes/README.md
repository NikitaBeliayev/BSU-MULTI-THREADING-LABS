# Lab №2 (Process creation)
<br>
The main goal of this lab is to learn how to create processes using the WINAPI library.

### Task description:

1. Сreate a parent process that should do the following tasks:
    - Enter array size, enter array elements
    - Generates a command line that contains information about the array dimension, elements
    - For the console of the child process, sets any text color
    - Launches the child process Child, to which information about the dimensions of the array,elements is passed through the command line
2. The child process must perform the following tasks:
    - Find elements in an array of numbers that are multiples of 5
    - Place them in the array on the left, the rest of the array elements on the right
    - Output the resulting array.

<br>
An example of entering array elements in the parent process:
<br>

``` cpp
int size = 0;
cout << "Enter size of array:  ";
cin >> size;
int* array = new int[size];
for (int i = 0; i < size; i++){ 
    cout << "Enter data for array[" + to_string(i) + "]: ";
    cin >> array[i];
}
```
<br>
Create command line for child process:
<br>

``` cpp
char child_process_name[] = "..\\..\\ChildProcess\\cmake-build-debug\\ChildProcess.exe";
STARTUPINFO startup_info;
PROCESS_INFORMATION process_information;
ZeroMemory(&startup_info, sizeof(STARTUPINFO));
startup_info.cb = sizeof(STARTUPINFO);
startup_info.dwFlags =STARTF_USEFILLATTRIBUTE;
startup_info.dwFillAttribute = FOREGROUND_BLUE;
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
Creating a child process and a new console for it:
<br>

``` cpp
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
```
<br>
Getting parameters from the parent process and converting the string to an array of numbers:
<br>

``` cpp
vector<int> vec;
vector<int> temp;
vector<int> result;
for (int i = 0 ; i < argc; i++){ 
    vec.push_back(std::atoi(argv[i]));
}
```
<br>
Accomplishment of the main task:
<br>

``` cpp
int count_multiple_5 = 0;
for (int i = 0;i < vec.size(); i++){ 
    if (vec[i] % 5 == 0){
        temp.push_back(vec[i]);
        count_multiple_5++;
    }
}
if (count_multiple_5 == 0){
    cout << "The are no elements multiple of 5 in the array";
    _getch();
    return 0;
}
for (int i = 0; i < temp.size(); i++){
    result.push_back(temp[i]);
}
for (int i = 0;i < vec.size(); i++){
    if (vec[i] % 5 != 0){
        result.push_back(vec[i]);
    }
}
for (int i = 0; i < result.size(); i++){
    cout << result[i] << " ";
}
_getch();
return 0;
```

<span class="colour" style="color:rgb(204, 120, 50)"></span>
