#include <iostream>
int main(){
    int n;
    std::cin >> n; // Read the bound giving n
    if (n < 2){
        return 0; // there are no primes below 2
    }

    int* p = new int[n]; // Allocate space for n integers
    // Standard C++ does not allow int p[n] because n is not known at compile time.
    // Some compilers accept this as an extension, but you should not rely on it.
    for (int i = 0; i < n; ++i){
        p[i] = 1;
    }
    p[0] = 0;
    p[1] = 0;
    for (int i = 2; i < n; ++i){
        if (p[i] == 1){
            for (int j = 2 * i; j < n; j += i){
                p[j] = 0;
            }
        }
    }
    for (int i = 0; i < n; ++i){
        if (p[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    delete[] p;
}
