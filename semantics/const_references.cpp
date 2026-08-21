int main() {
    int v = 5;
    int& p = v; // p is an alias for v
    p = 10; // Since p is an alias for v, v is also now 10

    const int& d = v; // d is an alias that cannot be used to modify v
    // d = 7; // Illegal
}
