#include <iostream>

int * foo() {
    // foo returns a pointer to where foo_arr is stored

    int * foo_arr = new int[100]; // we request to be allocated memory for 100 ints
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
    return foo_arr;
}

int main(){
    int * main_arr = foo();
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    if (main_arr[97] == 1){
        return 1; // early exit: main_arr is never freed
    }
    delete[] main_arr; // We are finished with using the allocated memory, we free it
    return 0;
}
