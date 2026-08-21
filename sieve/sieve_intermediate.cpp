#include <iostream>

// use dynamic storage duration (heap memory)

int * foo() {
    // a pointer variable is declared using the syntax <type>* <varname>
    // new[] returns a pointer to a block of memeory that has been allocated for our use, with enough capacity to store 100 integers.
    // When foo returns, it returns the pointer to this block of memory.
    // Notice that the memory has not yet been freed.
    // Hence main can access the entries in that block for printing.
    // After main is finished using it, it frees the memory with delete[]

    // foo returns a pointer to where foo_arr is storesd
    int * foo_arr = new int[100]; // we request to be allocated memory for 100 ints
    for (int i = 0; i < 100; ++i) {
        foo_arr[i] = 1; // 1 means possible prime, 0 means crossed out
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i) {
        if (foo_arr[i] == 1){ // i was never crossed out, it is prime
            for (in j = 2 * i; j < 100; j += i) {
                foo_arr[j] = 0; // cross out the multiples of i
            }
        }
    }
    return foo_arr;
}

int main() {
    int * main_arr = foo(); // main_arr points at the same heap block foo allocated
    for (int i = 0; i < 100; ++i) {
        if (main_arr[i] == 1) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    delete[] main_arr; // We are finished with using the allocated memory, we free it
    return 0;
}
