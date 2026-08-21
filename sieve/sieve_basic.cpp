// foo_arr is in stack memory, and not accessible in main function anymore

int foo() {
    // The array of 100 ints (typically 400 bytes) lives in stack memory
    int foo_arr[100];
    for (int i = 0; i < 100; ++i) {
        foo_arr[i] = 1; // 1 means possible prime, 0 means crossed out
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i) {
        if (foo_arr[i] == 1){ // i was never crossed out, it is prime
            for (in j = 2 * i; j < 100; j += i) {
                foo_arr[j] = 0; // cross out the multiples of i
            }
        }
    }
    int count = 0;
    for (int i = 0; i < 100; ++i) {
        count += foo_arr[i];
    }
    return count; // the number of primes below 100, which is 25
    // at the end of the function, the 400 bytes of memory allicated are freed from stack memory
}
int main() {
    auto a = foo();
    return 0;
}
