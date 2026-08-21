#include<iostream>
#include<memory>

auto foo(){
    // Request memory for 100 integers and store its ownership in foo_arr
    std::unique_ptr<int[]> foo_arr(new int[100]);
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr; // foo_arr is a std::unique_ptr<int[]>
}

int main(){
    // When foo returns, the std::unique_ptr is moved into main_arr.
    // Since the unique_ptr is alive, the memory is not freed.
    auto main_arr = foo();
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    if (main_arr[97] == 1){
        return 1; // early exit: main_arr goes out of scope, the memory is freed
    }
    return 0; // main_arr goes out of scope, the memory is freed
}
