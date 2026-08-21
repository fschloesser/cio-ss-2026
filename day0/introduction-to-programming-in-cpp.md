# Introduction to Programming in C++

Welcome to CIO-SS-2026! Throughout the course, we will be using C++. This markdown file is aimed at helping you get used to programming concepts in C++. We assume that you have programmed before, although not necessarily in C++.

Before we begin, I think it's important to explain the motivation behind this choice. SCIP, the MIP solver library that we will be using throughout this class, is actually written in C. To create a performant MIP solver, it is important to use a language that is close to the machine so that we can exploit the machine's capabilities fully. For reasons that we will see in a moment, coding in C, however, also brings a lot of quirks. To avoid these quirks while still getting the most out of the machine, we're going to code in C++. The aim is to use the abstractions that C++ provides to interface safely with SCIP's C code.

We will cover several things in this markdown. First, we will introduce control flow and C++ types, followed by functions, lambdas, and function objects. We will then discuss value and reference semantics and stack and heap memory. Finally, we will discuss RAII, interfacing with C libraries, virtual functions, and how the ranges and algorithms library can improve our code.

## Control Flow: Your First C++ Program

Create a file `collatz/collatz.cpp`. C++ source files carry the extension `.cpp`, while headers use `.h` or `.hpp`. Type in the following program, which generates the Collatz sequence. The control flow should be self-explanatory.

```cpp
#include <iostream>
#include <vector>

// execution start
int main() {
    int n;
    // read from stdin
    std::cin >> n;
    // create a array
    std::vector<int> series;
    // reserve capacity of 1024 positions
    series.reserve(1024);
    // add an element to the end
    series.push_back(n);
    // get the last element of the array
    while (series.back() != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        // add n to the end
        series.push_back(n);
    }
    /*
    * An older way of writing the range-based loop below is
    * for (int i = 0; i < std::ssize(series); ++i) {
    *     std::cout << series[i] << " ";
    * }
    */
    for (int x : series) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    return 0;
}
```

From the directory containing the `collatz` folder, compile and run the program with

```bash
cd collatz
clang++ -std=c++23 collatz.cpp -o collatz
./collatz
```

We will use `-std=c++23` throughout. Execution starts at `main`, whose return value is the exit status of the process. This can be queried via

```bash
echo $?
```

In this code, we are introduced to our first C++ container, `std::vector`. A vector is a dynamically sized array whose elements are stored contiguously in memory. We are also introduced to its basic operations. We can add elements via `push_back`, access an element using the `[]` operator, get the last element with `back()`, and get the number of elements via `std::size(<vector name>)`.

It is important to understand how a vector works. A vector has both a size—the number of elements it contains—and a capacity—the number of elements for which it currently has storage. When adding an element would exceed its capacity, the vector allocates a larger block and copies or moves all existing elements into it. A vector's capacity typically grows geometrically—for example, 2, 4, 8, and so on—although the exact growth strategy depends on the standard-library implementation. This can lead to inefficiency. Suppose we want to add 1000 elements. After inserting the first two elements, adding another may cause the vector to allocate space for four elements and copy or move the existing elements into it. Once we have inserted four elements, it may allocate space for eight and copy or move them again. We can then add another four elements before the vector becomes full, at which point another reallocation may occur. This process continues as the vector grows. For this reason, `std::vector` provides the member function `<vector name>.reserve(<number of elements>)`, which ensures that the vector has capacity for at least the given number of elements without changing its size.

## Types

We will limit ourselves to the following data types in this course. This section covers what you need to know about them. As with the Collatz program, try these examples yourself: create a file `types/types.cpp`, change into the `types` directory, type each program into the file in turn, and compile and run it from there.

```cpp
#include <iostream>
#include <format>
int main(){
    int i = 5; // Integers
    const int constant = 5; // A constant integer whose value cannot be changed
    // constant = 10; // Illegal

    double j = 10.0; // Double

    double k = i / 2; // i / 2 is integer division and yields 2, which is then converted to 2.0
    double l = i / 2.0; // i is converted to double before division, so the result is 2.5

    double arr[] = {1.0, 2.0, 3.0};

    // We can also alias types
    typedef double Real; // alternatively using Real = double;
    Real p = 5.0;

    // C++ programmers often use auto instead of declaring the type manually.
    // The type is deduced from the right-hand side.
    auto a = 5; // a is an int
    auto b = 2.5; // b is a double
    auto c = 5.0; // c is a double
    auto d = arr[1]; // d is equal to 2.0 and has type double

    auto pi = 3.14159265359;
    std::cout << std::format("Pi is {:.3f}",pi); // Prints Pi is 3.142, the last digit is rounded
    return 0;
}
```

Another use case for `auto` is return-type deduction:

```cpp
auto foo() { // The return type of foo is deduced from its return statement
    return 5.0; // 5.0 has type double, so the return type is double
}

int main() {
    auto f = foo(); // foo returns a double, hence f is a double.
    return 0;
}
```

Finally, we introduce strings and characters:

```cpp
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
    // c_str() provides a pointer to the null-terminated character array
    // stored by a C++ string.
    foo(p.c_str());
    return 0;
}
```

## Functions, Lambdas and Function Objects

We will consider three kinds of callable entities in C++: an ordinary function, a lambda, and a function object. As an exercise, we now write code that multiplies a number `a` by a factor `b` in all three ways. Create `multiply/multiply.cpp`.

```cpp
#include <iostream>

// 1. An ordinary function, both a and b are passed on every call
int multiply_by(int a, int b){
    return a * b;
}

// 3. A function object, a class whose operator() makes its objects callable like a function
class MultiplyBy{
public:
    MultiplyBy(int b) : b_(b) {} // the constructor stores the factor in the member b_
    int operator()(int a) const { return a * b_; }
private:
    int b_;
};

int main(){
    std::cout << multiply_by(5, 3) << "\n"; // 15

    // 2. A lambda, b is captured from the surrounding scope, only a is passed
    int b = 3;
    auto multiply_by_b = [b](int a){ return a * b; };
    std::cout << multiply_by_b(5) << "\n"; // 15

    MultiplyBy multiply_by_obj(3); // the factor is fixed once, at construction
    std::cout << multiply_by_obj(5) << "\n"; // 15
    return 0;
}
```

From the directory containing the `multiply` folder, compile and run the program with

```bash
cd multiply
clang++ -std=c++23 multiply.cpp -o multiply
./multiply
```

The ordinary function needs no explanation: both arguments arrive at every call. The lambda is for the situation where `b` is already fixed: the bracket `[b]` is the capture list, and the variables listed there are copied into the lambda at the point where it is defined. Changing `b` afterwards does not change `multiply_by_b`; writing `[&b]` instead would capture a reference to `b` rather than a copy. A lambda has a compiler-generated type with no name, hence `auto`.

The function object is the same idea spelled out by hand. `MultiplyBy` is a class with a member `b_`, a constructor that fills it, and an `operator()` that does the multiplication, so an object of this class is called like a function while carrying its factor around as state. The constructor syntax `: b_(b)` is new: it is called a member initializer list and sets the member `b_` to the argument `b` before the constructor body runs. In fact a lambda is exactly such a class written for us by the compiler, the capture list becoming the members.

## Value and Reference Semantics

We will now introduce two concepts in C++ called value and reference semantics.

The following is an example of value semantics:

```cpp
#include <iostream>

int add_five(int foo_v) {
    foo_v += 5;
    return foo_v;
}

int main() {
    int v = 10;
    v = add_five(v);
    std::cout << v << std::endl; // Prints 15
}
```

At the beginning of `main`, the variable `v` has the value 10. When `add_five` is called, the parameter `foo_v` is initialized with a copy of that value. The function adds 5 to its private copy and returns 15. The returned value is then assigned to `v`, replacing its previous value. This is a fundamental concept in C++: unless otherwise specified, function arguments are passed by value.

The following is an example of reference semantics:

```cpp
#include <iostream>

void add_five(int& foo_v) {
    foo_v += 5;
}

int main() {
    int v = 10;
    add_five(v);
    std::cout << v << std::endl; // Prints 15
}
```

At the beginning of `main`, the variable `v` again has the value 10. The `&` in the parameter type means that `foo_v` is a reference to, or alias for, `v`. Therefore, adding 5 to `foo_v` modifies `v` directly.

Which semantics we use can play a really important role in performance and correctness:

```cpp
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
        // Only the copy is changed; arr is unchanged.
    }

    cnt = 0;
    for (auto& v : arr) {
        // Here auto& is deduced as int&, so v is an alias for an element in arr.
        v = cnt++;
        // Changing v changes the corresponding element in arr.
    }
    // arr is now [0, 1, 2, 3, 4, 5, ..., 999]

    int sum1 = slow_sum(arr); // OH NO! We accidentally copied a vector of 1000 integers
    int sum2 = fast_sum(arr); // vec refers to arr, so no vector copy is made
}
```

We give a short remark about `const` references.

```cpp
int main() {
    int v = 5;
    int& p = v; // p is an alias for v
    p = 10; // Since p is an alias for v, v is also now 10

    const int& d = v; // d is an alias that cannot be used to modify v
    // d = 7; // Illegal
}
```

## Stack and Heap Memory

If one is to simplify computing, then the act of computing is the act of taking some data and performing some manipulations on it. Therefore, memory is a really important concept in programming. As a language that is close to the machine, C++ provides memory in different flavors. Programmers usually distinguish three storage durations: static, automatic, and dynamic. We will not discuss static storage duration for now. In general, when you want memory from the computer, you need to request it, which is also known as allocation. After you finish using it, the memory needs to be given back, which is also known as deallocation. The difference between automatic and dynamic storage duration is who requests the memory and who is responsible for giving it back. For the next exercise, create a folder `sieve` containing a file `sieve.cpp`, then change into the `sieve` directory. All commands and file replacements in this section assume that you remain in that directory.

We first introduce objects with automatic storage duration, which are typically stored in stack memory. The storage duration is called automatic because the memory is allocated when the object is declared and freed once the object goes out of scope.

```cpp
int foo(){
    // The array of 100 ints (typically 400 bytes) lives in stack memory
    int foo_arr[100];
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1; // 1 means possible prime, 0 means crossed out
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){ // i was never crossed out, it is prime
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0; // cross out the multiples of i
            }
        }
    }
    int count = 0;
    for (int i = 0; i < 100; ++i){
        count += foo_arr[i];
    }
    return count; // the number of primes below 100, which is 25
    // at the end of the function the 400 bytes of memory allocated are freed from stack memory automatically
}
int main(){
    auto a = foo();
    return 0;
}
```

There are several weaknesses in the above program. For example, if the caller wants the primes themselves instead of merely their count, then `foo_arr` is no longer available within the `main` function.

The above use case motivates dynamic storage duration: storage for which we want to control when it is allocated and when it is freed. Memory with dynamic storage duration is also often called heap memory. Replace the contents of `sieve.cpp` with the following and run it again.

```cpp
#include<iostream>

int * foo(){
    // foo returns a pointer to where foo_arr is stored
    int * foo_arr = new int[100]; // we request to be allocated memory for 100 ints
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr;
}

int main(){
    int * main_arr = foo(); // main_arr points at the same heap block foo allocated
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    delete[] main_arr; // We are finished with using the allocated memory, we free it
    return 0;
}
```

Here we encounter a pointer for the first time. A pointer variable is declared using the syntax `<type>* <varname>` (e.g. `int* main_arr`). In the above code, the function `foo` evaluates the expression `new int[100]`. `new[]` returns a pointer to a block of memory that has been allocated for our use, with enough capacity to store 100 integers. When `foo` returns, it returns the pointer to this block of memory. Notice that the memory has not yet been freed. Hence, `main` can access the entries in that block for printing. After `main` is finished using it, it frees the memory with `delete[]`.

These examples demonstrate how manual allocation works, but owning raw pointers and direct calls to `new[]` and `delete[]` should not be used in normal course code. Prefer `std::vector` for dynamic arrays and `std::unique_ptr` for other uniquely owned resources.

One can easily 'forget' to free allocated memory. We call this a memory leak. This can happen in a nontrivial way:

```cpp
#include<iostream>
int * foo(){
    // foo returns a pointer to where foo_arr is stored
    int * foo_arr = new int[100]; // we request to be allocated memory for 100 ints
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr;
}

int main(){
    int * main_arr = foo();
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    if (main_arr[97] == 1){
        return 1; // early exit: main_arr is never freed
    }
    delete[] main_arr; // We are finished with using the allocated memory, we free it
    return 0;
}
```

In the above code, since 97 is prime, the condition holds and we return without ever freeing `main_arr`. C++ offers us a type called `std::unique_ptr` that helps us avoid this.

```cpp
#include<iostream>
#include<memory>

auto foo(){
    // Request memory for 100 integers and store its ownership in foo_arr
    std::unique_ptr<int[]> foo_arr(new int[100]);
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr; // foo_arr is a std::unique_ptr<int[]>
}

int main(){
    // When foo returns, the std::unique_ptr is moved into main_arr.
    // Since the unique_ptr is alive, the memory is not freed.
    auto main_arr = foo();
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    if (main_arr[97] == 1){
        return 1; // early exit: main_arr goes out of scope, the memory is freed
    }
    return 0; // main_arr goes out of scope, the memory is freed
}
```

A second use case for heap memory arises when we do not know beforehand how many elements need to be allocated. Suppose we want the primes below a bound `n` that the user provides. Replace the contents of `sieve.cpp` with the following and run it again; the program now waits for you to type the bound and press enter.

```cpp
#include <iostream>
int main(){
    int n;
    std::cin >> n; // Read the bound giving n
    if (n < 2){
        return 0; // there are no primes below 2
    }

    int* p = new int[n]; // Allocate space for n integers
    // Standard C++ does not allow int p[n] because n is not known at compile time.
    // Some compilers accept this as an extension, but you should not rely on it.
    for (int i = 0; i < n; ++i){
        p[i] = 1;
    }
    p[0] = 0;
    p[1] = 0;
    for (int i = 2; i < n; ++i){
        if (p[i] == 1){
            for (int j = 2 * i; j < n; j += i){
                p[j] = 0;
            }
        }
    }
    for (int i = 0; i < n; ++i){
        if (p[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    delete[] p;
}
```

C++ gives us `std::vector` for this purpose: it allocates and frees heap memory for us. Again, replace the contents of `sieve.cpp` and run.


```cpp
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
```

We show how a vector can be used to solve the earlier problem. We propose two solutions; try each one in `sieve.cpp` as before.

```cpp
#include <vector>
#include <iostream>

auto foo(){
    std::vector<int> foo_arr(100, 1); // we request memory for 100 integers, all initialized to 1
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i]){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
    return foo_arr;
}

int main(){
    auto main_arr = foo(); // auto is deduced to std::vector<int>
    for (int i = 0; i < 100; ++i){
        if (main_arr[i]){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
```

In this first example, we allocate heap memory through `std::vector`. At the end of the function the buffer owned by `foo_arr` is moved to `main_arr`, and it is only when `main` ends that the memory is freed.

```cpp
#include <vector>
#include <iostream>
#include <span>
#include <cassert>

void foo(std::span<int> foo_arr){
    assert(foo_arr.size() == 100);
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
}

int main(){
    std::vector<int> main_arr(100);
    foo(main_arr); // a vector converts implicitly to a span over its elements
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
```
In the second example, heap memory is allocated and owned by `main`. `main` then 'shares' this memory with the `foo` function. A `std::span` is a non-owning view: it holds a pointer and a length, and frees nothing.

We note that a third solution is possible:

```cpp
#include <iostream>
#include <span>

void foo(std::span<int> foo_arr){
    for (int i = 0; i < 100; ++i){
        foo_arr[i] = 1;
    }
    foo_arr[0] = 0;
    foo_arr[1] = 0;
    for (int i = 2; i < 100; ++i){
        if (foo_arr[i] == 1){
            for (int j = 2 * i; j < 100; j += i){
                foo_arr[j] = 0;
            }
        }
    }
}

int main(){
    int main_arr[100];
    foo(main_arr); // a C array converts implicitly to a span of size 100
    for (int i = 0; i < 100; ++i){
        if (main_arr[i] == 1){
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
```

Here, `main_arr` is an array allocated in stack memory as part of the `main` function. Since `main` outlives `foo`, `main` can share this memory with `foo`. There are several reasons this may be undesirable. First, this solution presupposes that we know at compile time that we are only computing the primes below 100. If one day we need the primes below `n`, then we cannot simply put `n` instead of 100, as the note above suggests. Second, stack memory is generally limited compared with heap memory (often to a few megabytes per thread). Hence, dynamic storage is generally preferred for large arrays and arrays whose size is known only at runtime.

## RAII and Interfacing with C Libraries

`std::unique_ptr` enables a really powerful pattern called RAII, which we demonstrate here. A huge benefit of C++ is that it can call C libraries directly, and we will rely on this often. A C library typically hands out resources in counterpart pairs: one function acquires the resource, a second function must be called to give it back. Our example is SQLite, a database engine written in C: `sqlite3_open` opens a connection to a database file and `sqlite3_close` closes it. We write a program that reads a database filename from standard input, fills a small table of knapsack items and prints it. Create `sqlite/sqlite.cpp`.

```cpp
#include <iostream>
#include <sqlite3.h>
#include <string>

int main(){
    std::string filename;
    std::cin >> filename;

    // sqlite3_open is a C function, it needs a C string (recall the section Types)
    sqlite3 * db = nullptr;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
        std::cout << "Could not open " << filename << "\n";
        sqlite3_close(db); // sqlite3_open hands out a connection even on failure, and it too must be closed
        return 1;
    }

    const char * sql =
        "DROP TABLE IF EXISTS items;"
        "CREATE TABLE items (name TEXT, weight INT, value INT);"
        "INSERT INTO items VALUES ('hammer', 8, 30), ('rope', 3, 14), ('compass', 1, 25);"
        "SELECT * FROM items;";

    // the callback runs once per row of the SELECT
    auto print_row = [](void *, int ncols, char ** row, char **){
        for (int i = 0; i < ncols; ++i){
            std::cout << row[i] << " ";
        }
        std::cout << "\n";
        return 0;
    };
    sqlite3_exec(db, sql, print_row, nullptr, nullptr);

    sqlite3_close(db); // every sqlite3_open must be paired with a sqlite3_close
    return 0;
}
```

From the directory containing the `sqlite` folder, compile and run the program with

```bash
cd sqlite
clang++ -std=c++23 sqlite.cpp -o sqlite -lsqlite3
./sqlite
```

The new flag `-lsqlite3` tells the linker to link our program against the SQLite library; without it every `sqlite3_` function is an unresolved name. The program waits for you to type a filename and press enter; type `course.db`. SQLite creates the file if it does not exist. Afterwards you can inspect the database with the command line tool, `sqlite3 course.db` (type `.quit` to leave).

Two things in this program deserve a closer look. The first is the signature of `sqlite3_open`:

```cpp
int sqlite3_open(const char * filename, sqlite3 ** db);
```

The return value is already taken by the error code, so the function cannot also return the connection. Instead we declare a pointer `db` and pass its address `&db`, and the library writes the freshly built connection through it:

```
 db   : sqlite3 *   ── after the call, points at ──►  the connection object
&db   : sqlite3 **  ── lets sqlite3_open write into the variable db itself
```

A parameter used this way is called an out-parameter, and passing `&pointer` to receive a handle is the standard idiom of C libraries. It will come back many times in this course: SCIP, the solver we build on, creates its central object with exactly this shape, `SCIPcreate(&scip)`.

The second is the callback. `sqlite3_exec` wants a plain C function pointer that it can call once per row, and a C library knows nothing about C++ function objects. A lambda with an empty capture list `[]` converts to exactly such a function pointer. Recall from the section *Functions, Lambdas and Function Objects* that the captures become the members of a compiler-generated class; only a captureless lambda has no such state and can therefore act as a plain function.

We never look inside a `sqlite3`; we only hold a pointer to it and pass that pointer back to the library. C has no destructors, so the library trusts us to call the counterpart function `sqlite3_close` ourselves. One can easily 'forget' to do so, and this can happen in a nontrivial way. Suppose we decide that a failing query should abort the program.

```cpp
#include <iostream>
#include <sqlite3.h>
#include <string>

int main(){
    std::string filename;
    std::cin >> filename;

    sqlite3 * db = nullptr;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
        std::cout << "Could not open " << filename << "\n";
        sqlite3_close(db);
        return 1;
    }

    const char * sql =
        "DROP TABLE IF EXISTS items;"
        "CREATE TABLE items (name TEXT, weight INT, value INT);"
        "INSERT INTO items VALUES ('hammer', 8, 30), ('rope', 3, 14), ('compass', 1, 25);"
        "SELECT * FROM items;";

    auto print_row = [](void *, int ncols, char ** row, char **){
        for (int i = 0; i < ncols; ++i){
            std::cout << row[i] << " ";
        }
        std::cout << "\n";
        return 0;
    };
    if (sqlite3_exec(db, sql, print_row, nullptr, nullptr) != SQLITE_OK){
        std::cout << "Query failed: " << sqlite3_errmsg(db) << "\n";
        return 1; // early exit: sqlite3_close is never called, the connection stays open
    }

    sqlite3_close(db);
    return 0;
}
```

For a failing query, we return without ever closing the connection—put a typo into the SQL string and you will walk this path yourself. A database connection holds an open file, and the operating system limits how many files a process may hold open at once, so a program that keeps 'forgetting' eventually finds that every further open fails. One way to avoid this is to use a paradigm called RAII (Resource Acquisition Is Initialization): we tie the resource to an object whose destructor calls the counterpart function, so that every way out of the function, early or not, gives the resource back. We will use this paradigm whenever a function call must be paired with a counterpart that releases the acquired resource. Replace the contents of `sqlite.cpp` with the following, then compile and run it again as before.

```cpp
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>

struct SqliteCloser{
    // 1. We create a `promise` to close the connection once the owner goes out of scope
    void operator()(sqlite3 * db) const noexcept { sqlite3_close(db); }
};

int main(){
    std::string filename;
    std::cin >> filename;

    // 2. Open into a raw pointer, then immediately hand ownership over, with the promise attached
    sqlite3 * raw = nullptr;
    int rc = sqlite3_open(filename.c_str(), &raw);
    std::unique_ptr<sqlite3, SqliteCloser> db(raw);
    if (rc != SQLITE_OK){
        std::cout << "Could not open " << filename << "\n";
        return 1; // the promise still closes the handle that the failed open handed out
    }

    const char * sql =
        "DROP TABLE IF EXISTS items;"
        "CREATE TABLE items (name TEXT, weight INT, value INT);"
        "INSERT INTO items VALUES ('hammer', 8, 30), ('rope', 3, 14), ('compass', 1, 25);"
        "SELECT * FROM items;";

    auto print_row = [](void *, int ncols, char ** row, char **){
        for (int i = 0; i < ncols; ++i){
            std::cout << row[i] << " ";
        }
        std::cout << "\n";
        return 0;
    };
    // 3. .get() hands the raw pointer to a C function that knows nothing about ownership
    if (sqlite3_exec(db.get(), sql, print_row, nullptr, nullptr) != SQLITE_OK){
        std::cout << "Query failed: " << sqlite3_errmsg(db.get()) << "\n";
        return 1; // early exit needs no special case, the promise is kept
    }
    return 0; // db goes out of scope, the promise is kept, sqlite3_close runs
}
```

`SqliteCloser` is a function object as in the previous section; `std::unique_ptr` executes it on the owned pointer when the variable goes out of scope. Note the two-step opening: the out-parameter forces us to open into a raw pointer first and hand ownership over on the very next line. From that line on every way out of `main` closes the connection, including the error branch of the open itself. The method `.get()` exists precisely for interfacing with C: it hands out the raw pointer without giving up ownership. Any C library that comes in acquire/release pairs is wrapped in exactly this way.

## Virtual Functions: Polymorphism

Now that we have introduced pointers, ownership, and RAII, we can use them to study another important concept in C++: runtime polymorphism, also called dynamic dispatch. To demonstrate this, we write a program that draws shapes. There are three kinds—circles, squares, and triangles—and each draws itself differently. We will also split our program into multiple files, so create the folder `shapes` with the following contents.

```
shapes/
├── CMakeLists.txt
├── shape.hpp
├── circle.hpp
├── circle.cpp
├── square.hpp
├── square.cpp
├── triangle.hpp
├── triangle.cpp
└── main.cpp
```

A common convention in C++ is one class per header and source pair. The two provide a separation of concerns. The header is a contract: it tells users what capabilities the code provides. The source describes how those capabilities are implemented. We first introduce the class `Shape`, which describes what every shape has in common and nothing more.

```cpp
#pragma once

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};
```


We will not discuss all of the syntax in detail. The function `draw` is pure virtual because it is declared with `= 0`. This makes `Shape` an abstract class, so it cannot be instantiated directly. A concrete class such as `Circle` or `Square` must provide its own implementation of `draw`. The `const` after `draw()` means that the function does not modify the object on which it is called. The virtual destructor allows an object of a derived class to be destroyed safely through a pointer to `Shape`. The line `#pragma once` prevents the header from being included more than once in a translation unit.

A concrete shape inherits from `Shape` and supplies the missing `draw`. We first write the circle.

```cpp
// circle.hpp
#pragma once
#include "shape.hpp"

class Circle : public Shape {
public:
    void draw() const override;
};
```

```cpp
// circle.cpp
#include "circle.hpp"
#include <iostream>

void Circle::draw() const {
    std::cout << "Hey I am a circle\n";
}
```

Note that we write `#include "circle.hpp"` with quotes instead of angle brackets. Quotes mean look next to this file, angle brackets mean look where the compiler keeps its own headers. The keyword `override` is not decoration: the compiler rejects it if nothing in the base class matches, which catches a mistyped signature that would otherwise silently declare a new unrelated function.

The remaining two shapes are the same lines with the word substituted.

```cpp
// square.hpp
#pragma once
#include "shape.hpp"

class Square : public Shape {
public:
    void draw() const override;
};

// square.cpp
#include "square.hpp"
#include <iostream>

void Square::draw() const {
    std::cout << "Hey I am a square\n";
}

// triangle.hpp
#pragma once
#include "shape.hpp"

class Triangle : public Shape {
public:
    void draw() const override;
};

// triangle.cpp
#include "triangle.hpp"
#include <iostream>

void Triangle::draw() const {
    std::cout << "Hey I am a triangle\n";
}
```

Finally, in `main.cpp` we build ten shapes, each picked at random, and ask every one of them to draw itself.

```cpp
#include "circle.hpp"
#include "square.hpp"
#include "triangle.hpp"

#include <memory>
#include <random>
#include <vector>

int main(){
    std::mt19937 gen(std::random_device{}()); // a random number generator, seeded once
    std::uniform_real_distribution<double> u(0.0, 1.0);

    // 1. We create a vector that will own the ten shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    for (int i = 0; i < 10; ++i){
        double x = u(gen); // uniformly distributed in [0,1)
        // 2. We create a random shape in heap memory and hand ownership over to the vector
        if (x < 1.0/3.0){
            shapes.push_back(std::make_unique<Circle>());
        } else if (x < 2.0/3.0){
            shapes.push_back(std::make_unique<Square>());
        } else {
            shapes.push_back(std::make_unique<Triangle>());
        }
    }

    for (const auto & shape : shapes){
        // 3. Which draw runs is decided by the object, not by the pointer type
        shape->draw();
    }
    return 0; // the vector goes out of scope and every shape is destroyed, no delete written anywhere
}
```

We go through this example step by step. In the last loop, `shape` is a reference to a `std::unique_ptr<Shape>`. As we saw in the previous sections, a `std::unique_ptr` owns the object to which it points and provides the pointer operators `*` and `->`. Member access through a pointer is written with an arrow instead of a dot: `shape->draw()` is equivalent to `(*shape).draw()`—first dereference the pointer, then call `draw`. The dot used in expressions such as `p.c_str()` operates directly on an object.

Now, `Shape` does not know how to draw anything. Nevertheless, the correct message is printed for each element. Which `draw` is executed is decided at runtime from the concrete object being pointed to. We call this dynamic dispatch. In C++, this behavior is requested through a virtual function.

This is also why the vector holds pointers instead of shapes. `std::vector<Shape>` is not possible because `Shape` is abstract. Even with a non-abstract base class, storing derived objects by value in such a vector would slice away their derived parts. Instead, the shapes themselves live in heap memory, while the vector stores owning pointers of the common type `std::unique_ptr<Shape>`.

`std::make_unique<Circle>()` creates a `Circle` in heap memory and returns a `std::unique_ptr<Circle>`. When it is inserted into the vector, this is converted to a `std::unique_ptr<Shape>`. As discussed in the sections on stack and heap memory and RAII, a `std::unique_ptr` owns what it points to: when the pointer is destroyed, so is the shape. Unique ownership means exactly one owner at a time, so the pointer is moved into the vector rather than copied, and the program contains no `delete`. At the end of `main`, the vector is destroyed, which destroys the ten pointers, each of which destroys its shape.

The destructor in `shape.hpp` is what makes this safe. The vector destroys a `Circle` through a pointer to `Shape`, so `~Shape` must be virtual. Deleting a derived object through a base-class pointer whose destructor is not virtual results in undefined behavior. Compilers can often warn about this mistake.

To compile the program we must now hand four source files to `clang++`. This becomes tiresome quickly, so we use CMake instead. Put the following in `CMakeLists.txt` and take it as magic for now.

```cmake
cmake_minimum_required(VERSION 4.0)
project(shapes CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(draw main.cpp circle.cpp square.cpp triangle.cpp)
```

Note that every `.cpp` is listed and no header is. Headers are pasted into the sources by `#include`; it is the sources that are compiled, each on its own, and then linked together into one executable.

Now it is time to compile and run. In the terminal, go to the folder `shapes` and run

```bash
cmake -S . -B build
cmake --build build
./build/draw
```


## The Ranges and Algorithms Library

We close with the algorithms library, which recovers much of the comfort you are used to from Python: `sorted` with a key and `functools.reduce` both have counterparts here.  The goal of this example is to show how algorithms can be written more concisely and clearly by building on the existing language library functionalities. Our example is the knapsack problem. We are given items, each with a weight and a value, and a knapsack of capacity 25, and we want to pack as much value as possible into it. We generate 10 random items and try the classic greedy heuristic: pack the items with the best value per weight first. Create `knapsack/knapsack.cpp`.

```cpp
#include <algorithm>
#include <format>
#include <iostream>
#include <random>
#include <vector>

struct KnapsackItem {
    int id;
    int weight;
    int value;
};

// A generator: a function object whose every call produces the next random item
class KnapsackItemGenerator {
public:
    KnapsackItem operator()(){
        return KnapsackItem{next_id_++, weight_dist_(gen_), value_dist_(gen_)};
    }
private:
    std::mt19937 gen_{std::random_device{}()};
    std::uniform_int_distribution<int> weight_dist_{1, 10};
    std::uniform_int_distribution<int> value_dist_{1, 100};
    int next_id_ = 0;
};

double get_value_to_weight_ratio(const KnapsackItem & item){
    return static_cast<double>(item.value) / item.weight;
}

void print_knapsack_item(const KnapsackItem & item){
    std::cout << std::format("item {:2}: value {:3} weight {:2}\n", item.id, item.value, item.weight);
}

// The state of the packing: what has been packed so far and the capacity that is left
struct PackingState {
    std::vector<KnapsackItem> packed;
    int remaining_capacity;
};

int main(){
    // a knapsack of capacity 25 and 10 random items with ids 0,1,2,...
    int capacity = 25;
    std::vector<KnapsackItem> items(10);
    std::ranges::generate(items.begin(), KnapsackItemGenerator{});

    // Print the items
    std::cout << "Knapsack capacity: " << capacity << '\n';
    std::cout << "generated items: \n";
    std::ranges::for_each(items, print_knapsack_item);

    // sort the items by value per weight, best first
    std::ranges::sort(items, std::ranges::greater{}, get_value_to_weight_ratio);

    // Consider the current packing state and the next item: if the item fits, pack it, otherwise do not
    auto pack_if_fit = [](PackingState state, const KnapsackItem & item){
        if (item.weight <= state.remaining_capacity){
            state.packed.push_back(item);
            state.remaining_capacity -= item.weight;
        }
        return state;
    };
    // greedily pack the knapsack by folding over the items, best first
    PackingState final_state = std::ranges::fold_left(items, PackingState{{}, capacity}, pack_if_fit);
    
    std::vector<KnapsackItem> picked_item;

    using namsespace std;
    std::cout << "packed items:\n";
    ranges::for_each(final_state.packed, print_knapsack_item);
    std::cout << std::format("packed weight {}\n", capacity - final_state.remaining_capacity);

    return 0;
}
```
A `struct` is a class whose members are public by default, and `KnapsackItem{next_id_++, weight_dist_(gen_), value_dist_(gen_)}` fills the members in declaration order. `KnapsackItemGenerator` is a function object with state — the random number generator, the distributions, and a counter — so every call produces the next item; since the members are initialized at their declaration, no constructor is needed.

`std::ranges::sort` takes a comparator and a projection: the projection computes a key for every element, the comparator orders the keys. With `get_value_to_weight_ratio` as the projection and `std::ranges::greater{}` as the comparator this is `sorted(items, key=get_value_to_weight_ratio, reverse=True)`. The id records where each item started, since the sort shuffles them. The algorithms library holds many more, `std::ranges::max_element`, `std::ranges::count_if`, `std::ranges::find`, and reaching for them before writing a loop by hand is considered good style.

The packing is `functools.reduce`: `fold_left` threads a `PackingState` — the packed items and the remaining capacity — through `pack`, one call per item. An item that does not fit is skipped rather than terminating the packing process; a later, lighter one may still be packed. The packed weight falls out of the final state as `capacity - final_state.remaining_capacity`.

All three kinds of callables from the section *Functions, Lambdas and Function Objects* have appeared: ordinary functions in `for_each` and as the sort's projection, a function object in `generate`, a lambda in `fold_left`.

Note that the greedy packing is a heuristic. It is not, in general, the optimal solution of the knapsack problem — finding that is much harder, and this course is about exactly such problems.


