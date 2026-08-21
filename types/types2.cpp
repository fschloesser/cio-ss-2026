auto foo() { // The return type of foo is deduced from its return statement
    return 5.0; // 5.0 has type double, so the return type is double
}

int main() {
    auto f = foo(); // foo returns a double, hence f is a double
    return 0;
}
