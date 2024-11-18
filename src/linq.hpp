#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace linq {
    template<typename T>
    concept Rangeable = std::ranges::input_range<T>;

    template<Rangeable Range, typename Pred>
    constexpr auto where(Range&& range, Pred&& pred) {
        return std::ranges::ref_view{range} | std::views::filter(std::forward<Pred>(pred));
    }

    template<Rangeable Range, typename Proj>
    constexpr auto select(Range&& range, Proj&& proj) {
        return std::ranges::ref_view{range} | std::views::transform(std::forward<Proj>(proj));
    }

    template<Rangeable Range>
    constexpr auto toVector(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        std::vector<T> res;
        if constexpr (std::ranges::sized_range<Range>) {
            res.reserve(std::ranges::size(range));
        }
        
        std::ranges::copy(range, std::back_inserter(res));
        return res;
    }

    template<Rangeable Range, typename KeySelector>
    constexpr auto orderBy(Range&& range, KeySelector&& ks) {
        auto sorted = toVector(range);
        std::ranges::sort(sorted, {}, std::forward<KeySelector>(ks));

        return sorted;
    }

    template<Rangeable Range, typename T, typename Func>
    constexpr auto aggregate(Range&& range, T init, Func&& func) {
        return std::accumulate(range.begin(), range.end(), std::move(init), std::forward<Func>(func));
    }

    template<Rangeable Range, typename Pred = std::nullptr_t>
    constexpr auto count(Range&& range, Pred&& pred = nullptr) {
        if constexpr (std::is_same_v<Pred, std::nullptr_t>) {
            return std::ranges::distance(range);
        } else {
            return std::ranges::distance(where(range, std::forward<Pred>(pred)));
        }
    }

    template<Rangeable Range>
    constexpr auto sum(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        return std::accumulate(range.begin(), range.end(), T{});
    }

    template<Rangeable Range>
    constexpr auto min(Range&& range) {
        return std::ranges::min(range);
    }

    template<Rangeable Range>
    constexpr auto max(Range&& range) {
        return std::ranges::max(range);
    }

    template<Rangeable Range>
    constexpr auto first(Range&& range) -> decltype(*range.begin()) {
        if (range.begin() == range.end()) {
            throw std::out_of_range("Range is empty");
        }
        return *range.begin();
    }

    template<Rangeable Range>
    constexpr auto firstOrDefault(Range&& range) -> std::optional<std::ranges::range_value_t<Range>> {
        if (range.begin() == range.end()) {
            return std::nullopt;
        }
        return *range.begin();
    }

    template<Rangeable Range>
    constexpr auto last(Range&& range) -> decltype(*range.begin()) {
        if (range.begin() == range.end()) {
            throw std::out_of_range("Range is empty");
        }
        return *std::ranges::prev(range.end());
    }

    template<Rangeable Range>
    constexpr auto lastOrDefault(Range&& range) -> std::optional<std::ranges::range_value_t<Range>> {
        if (range.begin() == range.end()) {
            return std::nullopt;
        }
        return *std::ranges::prev(range.end());
    }

    template<Rangeable Range>
    constexpr auto distinct(Range&& range) {
        using T = std::ranges::range_value_t<Range>;

        std::unordered_set<T> seen;
        std::vector<T> res;

        for (const auto& elem: range) {
            if (seen.insert(elem).second) {
                res.push_back(elem);
            }
        }

        return res;
    }

    template<Rangeable Range, typename KeySelector>
    constexpr auto distinctBy(Range&& range, KeySelector&& ks) {
        using T = std::ranges::range_value_t<Range>;
        using K = std::invoke_result_t<KeySelector, T>;

        std::unordered_set<K> seen;
        std::vector<T> res;

        for (const auto& elem: range) {
            K key = ks(elem);
            if (seen.insert(key).second) {
                res.push_back(elem);
            }
        }

        return res;
    }

    template<Rangeable Range>
    constexpr auto take(Range&& range, std::size_t n) {
        return std::views::take(range, n);
    }
} // namespace linq
