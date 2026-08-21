#include <iostream>
#include <format>

int main() {
    int i = 5; // Integers
    const int contant = 5; // A constant integer whose value cannot be changed
    // contant = 10; // Illegal

    double j = 10.0; // Double

    double k = i / 2; // i / 2 is integer division and yields 2, which is then converted to 2.0
    double l = i / 2.0; // i is converted to double before division, so the result is 2.5

    double arr[] = {1.0, 2.0, 3.0};

    // We can also alias types
    typedef double Real; // alternatively using Real = double;
    Real p = 5.0;

    // C++ programmers often use auto instead of declaring the type manually
    // the type is deduced from the right-hand side.
    auto a = 5; // a is an int
    auto b = 2.5; // b is a double
    auto c = 5.0; // c is a double
    auto d = arr[1]; // d is equal to 2.0 and has type double

    auto pi = 3.14159265359;
    std::cout << std::format("Pi is {:.3f}",pi); // Prints Pi is 3.142, the last digit is rounded

    return 0;
}
