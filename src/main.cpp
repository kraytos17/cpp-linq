#include <print>
#include <string>
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
    for (int n: filtered)
        std::print("{} ", n);
    std::print("\n");

    auto squared = linq::select(numbers, [](int n) { return n * n; });
    std::print("Squares of numbers: ");
    for (int n: squared)
        std::print("{} ", n);
    std::print("\n");

    auto sorted = linq::orderBy(numbers, [](int n) { return n; });
    std::print("Numbers sorted in ascending order: ");
    for (int n: sorted)
        std::print("{} ", n);
    std::print("\n");

    std::print("Product of all numbers: {}\n", linq::aggregate(numbers, 1, std::multiplies<int>()));
    std::print("Count of numbers greater than 5: {}\n", linq::count(numbers, [](int n) { return n > 5; }));
    std::print("Sum of all numbers: {}\n", linq::sum(numbers));

    std::print("Minimum number: {}\n", linq::min(numbers));
    std::print("Maximum number: {}\n", linq::max(numbers));

    auto first = linq::first(numbers).value_or(0);
    std::print("First element: {}\n", first);

    auto firstOrDefault = linq::firstOrDefault(numbers);
    std::print("First element or default: {}\n", firstOrDefault);

    try {
        std::print("Last element: {}\n", linq::last(numbers).value());
    } catch (const std::out_of_range&) {
        std::print("Last element: Out of range\n");
    }

    auto lastOrDefault = linq::lastOrDefault(numbers);
    std::print("Last element or default: {}\n", lastOrDefault);

    std::vector<Person> people{{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"Alice", 40}, {"Eve", 28}};

    auto uniqueByName = linq::distinctBy(people, [](const Person& p) { return p.name; });
    std::print("Distinct people by name:\n");
    for (const auto& person: uniqueByName) {
        std::print("  {} ({} yrs)\n", person.name, person.age);
    }

    return 0;
}
