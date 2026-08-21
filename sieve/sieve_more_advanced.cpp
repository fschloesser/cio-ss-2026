#include <vector>
#include <iostream>
int main(){
    int n;
    std::cin >> n; // Read the bound giving n
    if (n < 2){
        return 0; // there are no primes below 2
    }
    std::vector<int> p(n, 1); // n integers, all initialized to 1
    p[0] = 0;
    p[1] = 0;
    for (int i = 2; i < n; ++i){
        if (p[i]){
            for (int j = 2 * i; j < n; j += i){
                p[j] = 0;
            }
        }
    }
    for (int i = 0; i < n; ++i){
        if (p[i]){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    // p is automatically freed here
}
