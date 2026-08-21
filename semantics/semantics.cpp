#include <iostream>
#include <vector>

int slow_sum(std::vector<int> vec) {
    int acc = 0;
    for (auto v : vec) {
        acc += v;
    }
    return acc;
}

int fast_sum(const std::vector<int>& vec) {
    int acc = 0;
    for (auto v : vec) {
        acc += v;
    }
    return acc;
}

int main() {
    std::vector<int> arr(1000, 1); // Create a vector of 1000 integers filled with ones

    int cnt = 0;
    for (auto v : arr) {
        // Since arr is a vector of int, auto is deduced as int.
        // On each iteration, v is a copy of an element in arr.
        v = cnt++;
        // Only the copy is changed; arr is unchanged
    }

    cnt = 0;
    for (auto& v : arr) {
        // Here auto& is deduced as int&, so v is an alias for an element in arr.
        v = cnt++;
        // Changing v changes the corresponding element in arr.
    }
    // arr is now [0, 1, 2, 3, 4, 5, ..., 999]

    int sum1 = slow_sum(arr); // OH NO! We accidentally copied a vector of 1000 integers
    int sum2 = fast_sum(arr); // vec refers to arr, so no vector copy is made.
}
