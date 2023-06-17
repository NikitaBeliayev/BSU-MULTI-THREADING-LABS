#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
using namespace std;

int main(int argc, char* argv[]) {
    vector<int> vec ;
    vector<int> temp;
    vector<int> result;
    for (int i = 0 ; i < argc; i++){
        vec.push_back(std::atoi(argv[i]));
    }
    int count_multiple_5 = 0;
    for(int i = 0;i < vec.size(); i++){
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

    for(int i = 0;i < vec.size(); i++){
        if (vec[i] % 5 != 0){
            result.push_back(vec[i]);
        }
    }

    for (int i = 0; i < result.size(); i++){
        cout << result[i] << " ";
    }
    _getch();
    return 0;
}