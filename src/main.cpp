#include <print>
#include <vector>
#include "linq.hpp"

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<int> numbers{1, 3, 5, 2, 8, 6, 7, 4, 10, 9};

    auto filtered = linq::where(numbers, [](int n) { return n > 5; });
    std::print("Numbers greater than 5: ");
    for (int n: filtered) {
        std::print("{} ", n);
    }
    std::println();

    auto squared = linq::select(numbers, [](int n) { return n * n; });
    std::print("Squares of numbers: ");
    for (int n: squared) {
        std::print("{} ", n);
    }
    std::println();

    auto sorted = linq::orderBy(numbers, [](int n) { return n; });
    std::print("Numbers sorted in ascending order: ");
    for (const auto& n: sorted) {
        std::print("{} ", n);
    }
    std::println();

    int product = linq::aggregate(numbers, 1, std::multiplies<int>());
    std::println("Product of all numbers: {}", product);

    int countGreater5 = linq::count(numbers, [](int n) { return n > 5; });
    std::println("Count of numbers greater than 5: {}", countGreater5);

    int sum = linq::sum(numbers);
    std::println("Sum of all numbers: {}", sum);

    int minNumber = linq::min(numbers);
    std::println("Minimum number: {}", minNumber);

    int maxNumber = linq::max(numbers);
    std::println("Maximum number: {}", maxNumber);

    try {
        int first = linq::first(numbers);
        std::println("First element: {}", first);
    } catch (const std::out_of_range& e) {
        std::println("Error: {}", e.what());
    }

    auto firstOrDefault = linq::firstOrDefault(numbers);
    std::println("First element or default: {}", firstOrDefault.value());

    try {
        int last = linq::last(numbers);
        std::println("Last element: {}", last);
    } catch (const std::out_of_range& e) {
        std::println("Error: {}", e.what());
    }

    auto lastOrDefault = linq::lastOrDefault(numbers);
    std::println("Last element or default: {}", lastOrDefault.value());

    std::vector<Person> people{{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"Alice", 40}, {"Eve", 28}};
    auto uniqueByName = linq::distinctBy(people, [](const Person& p) { return p.name; });
    std::println("Distinct people by name:\n");
    for (const auto& person: uniqueByName) {
        std::println("{0} ({1} yrs)", person.name, person.age);
    }

    return 0;
}
