#include <vector>
#include <iostream>

auto foo(){
    std::vector<int> foo_arr(100, 1); // we request memory for 100 integers, all initialized to 1
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i]){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr;
}

int main(){
    auto main_arr = foo(); // auto is deduced to std::vector<int>
    for (int i = 0; i < 100; ++i){
        if (main_arr[i]){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
