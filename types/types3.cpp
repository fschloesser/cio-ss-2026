#include <string>
#include <iostream>

void foo(const char* s) {
    std::cout << "This is a function that needs a C string. And the string is "
              << s << std::endl;
}

int main() {
    const char* c = "This is a C string";
    std::string p = "This is a C++ string";

    foo(c);
    // c_str() provides a pointer to the null-terminated character array stored by a C++ string.
    foo(p.c_str());
    return 0;
}
