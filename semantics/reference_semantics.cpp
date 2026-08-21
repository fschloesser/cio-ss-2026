#include <iostream>

// this is an example of reference semantics

// & in the parameter type means that foo_v is a reference to, or alias for v.
// Therefore, adding 5 to foo_v modifies v directly.
void add_five(int& foo_v) {
    foo_v += 5;
}

int main() {
    int v = 10;
    add_five(v);
    std::cout << v << std::endl; // Prints 15
}
