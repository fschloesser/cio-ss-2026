#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> series;
    series.reserve(1024);
    series.push_back(n);
    while (series.back() != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
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
