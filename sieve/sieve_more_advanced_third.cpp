#include <iostream>
#include <span>

void foo(std::span<int> foo_arr){
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
}

int main(){
    int main_arr[100];
    foo(main_arr); // a C array converts implicitly to a span of size 100
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
