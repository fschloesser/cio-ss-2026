#include <iostream>

// 1. An ordinary function, both a and b are passed on every call
int multiply_by(int a, int b){
    return a * b;
}

// 3. A function object, a class whose operator() makes its objects callable like a function
// MultiplyBy is a calss with a member b_, a constructor that fills it, and an operator operator() that does the multiplication
class MultiplyBy{
    public:
        // constructor syntax : b_(b) is new: it is called a memeber initializer list and sets the member b_ to the argument b before the constructor body runs.
        // In fact a lambda is exactly such a class written for us by the compiler, the capture list becoming the members.
        MultiplyBy(int b) : b_(b) {} // the constructor stores the factor in the member b_
        int operator() (int a) const { return a * b_; }
    private:
        int b_;
};

int main() {
    std::cout << multiply_by(5, 3) << "\n"; // 15

    // 2. A lambda, b is captured from the surrounding scope, only a is passed
    int b = 3;
    // bracket [b] is the capture list, and the variables listed there are copied int the lambda at the point where it is defined. i
    // Changing b afterwards does not change multiply_by_b;
    // writing [&b] instead would capture a reference to b rather than a copy.
    // A lamda has a compiler-generated type with no name, hence auto.
    auto multiply_by_b = [b](int a){ return a * b; };
    std::cout << multiply_by_b(5) << "\n"; // 15

    MultiplyBy multiply_by_obj(3); // the factor is fixed once, at construction
    std::cout << multiply_by_obj(5) << "\n"; // 15
    return 0;
}
