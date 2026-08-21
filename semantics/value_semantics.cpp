#include <iostream>

// This is an example of value semantics

// unless otherwise specified, function arguments are passed by value:
// when add_five is called, foo_v is initialized with a copy of that value.
// The function adds 5 to its private copy and returns 15.
// The returned value is then assigned to v, replacing its previous value.
int add_five(int foo_v) {
    foo_v += 5;
    return foo_v;
}

int main() {
    int v = 10;
    v = add_five(v);
    std::cout << v << std::endl; // Prints 15
}
