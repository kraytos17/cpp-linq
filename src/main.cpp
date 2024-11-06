#include <iostream>
#include <vector>
#include "linq.hpp"

int main() {
    std::vector<int> numbers{1, 3, 5, 2, 8, 6, 7, 4, 10, 9};

    auto filtered = linq::where(numbers, [](int n) { return n > 5; });
    std::cout << "Numbers greater than 5: ";
    for (int n: filtered) {
        std::cout << n << " ";
    }

    std::cout << "\nSquares of numbers: ";
    auto squared = linq::select(numbers, [](int n) { return n * n; });
    for (int n: squared) {
        std::cout << n << " ";
    }

    std::cout << "\n";
    auto sorted = linq::orderBy(numbers, [](int n) { return n; });
    std::cout << "Numbers sorted in ascending order (vector): ";
    for (const auto& n: sorted) {
        std::cout << n << " ";
    }

    std::cout << "\n";
    return 0;
}
