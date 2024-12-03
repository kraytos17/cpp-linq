#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace linq {
    template<typename T>
    concept Rangeable = std::ranges::input_range<T>;

    template<Rangeable Range, typename Pred>
    auto where(Range&& range, Pred&& pred) noexcept {
        return std::ranges::ref_view{range} | std::views::filter(std::forward<Pred>(pred));
    }

    template<Rangeable Range, typename Proj>
    auto select(Range&& range, Proj&& proj) noexcept {
        return std::ranges::ref_view{range} | std::views::transform(std::forward<Proj>(proj));
    }

    template<Rangeable Range>
    auto toVector(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        std::vector<T> res;
        if constexpr (std::ranges::sized_range<Range>) {
            res.reserve(std::ranges::size(range));
        }
        std::ranges::copy(range, std::back_inserter(res));
        return res;
    }

    template<Rangeable Range, typename KeySelector>
    auto orderBy(Range&& range, KeySelector&& ks) {
        auto sorted = toVector(std::forward<Range>(range));
        std::ranges::sort(sorted, {}, std::forward<KeySelector>(ks));
        return sorted;
    }

    template<Rangeable Range, typename T, typename Func>
    auto aggregate(Range&& range, T init, Func&& func) {
        return std::accumulate(range.begin(), range.end(), std::move(init), std::forward<Func>(func));
    }

    template<Rangeable Range, typename Pred = std::nullptr_t>
    auto count(Range&& range, Pred&& pred = nullptr) noexcept {
        if constexpr (std::is_same_v<Pred, std::nullptr_t>) {
            if constexpr (std::ranges::sized_range<Range>) {
                return std::ranges::size(range);
            } else {
                return std::ranges::distance(range);
            }
        } else {
            return std::ranges::count_if(range, std::forward<Pred>(pred));
        }
    }

    template<Rangeable Range>
    auto sum(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        return std::accumulate(range.begin(), range.end(), T{});
    }

    template<Rangeable Range>
    auto min(Range&& range) {
        return std::ranges::min(range);
    }

    template<Rangeable Range>
    auto max(Range&& range) {
        return std::ranges::max(range);
    }

    template<Rangeable Range>
    auto first(Range&& range) -> std::optional<std::ranges::range_value_t<Range>> {
        auto it = std::ranges::begin(range);
        if (it != std::ranges::end(range)) {
            return *it;
        }
        return std::nullopt;
    }

    template<Rangeable Range>
    auto firstOrDefault(Range&& range,
                        std::ranges::range_value_t<Range> defaultValue = {}) -> std::ranges::range_value_t<Range> {
        auto it = std::ranges::begin(range);
        if (it != std::ranges::end(range)) {
            return *it;
        }
        return defaultValue;
    }

    template<Rangeable Range>
    auto last(Range&& range) -> std::optional<std::ranges::range_value_t<Range>> {
        if (range.begin() == range.end()) {
            return std::nullopt;
        }
        return *std::ranges::prev(range.end());
    }

    template<Rangeable Range>
    auto lastOrDefault(Range&& range,
                       std::ranges::range_value_t<Range> defaultValue = {}) -> std::ranges::range_value_t<Range> {
        if (range.begin() == range.end()) {
            return defaultValue;
        }
        return *std::ranges::prev(range.end());
    }

    template<Rangeable Range>
    auto distinct(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        std::unordered_set<T> seen;
        std::vector<T> result;

        if constexpr (std::ranges::sized_range<Range>) {
            result.reserve(std::ranges::size(range));
        }

        for (auto&& elem: range) {
            if (seen.insert(elem).second) {
                result.push_back(elem);
            }
        }

        return result;
    }

    template<Rangeable Range, typename KeySelector>
    auto distinctBy(Range&& range, KeySelector&& ks) {
        using T = std::ranges::range_value_t<Range>;
        using K = std::invoke_result_t<KeySelector, T>;

        std::unordered_set<K> seen;
        std::vector<T> result;

        if constexpr (std::ranges::sized_range<Range>) {
            result.reserve(std::ranges::size(range));
        }

        for (auto&& elem: range) {
            K key = ks(elem);
            if (seen.insert(key).second) {
                result.push_back(std::forward<decltype(elem)>(elem));
            }
        }

        return result;
    }

    template<Rangeable Range>
    auto take(Range&& range, std::size_t n) {
        return std::views::take(std::forward<Range>(range), n);
    }

    template<Rangeable ORange, Rangeable IRange, typename OKeyS, typename IKeyS, typename TRes>
    auto join(const ORange& outer, const IRange& inner, OKeyS outerKeySel, IKeyS innerKeyS, TRes resSelector) {
        using OuterVal = std::ranges::range_value_t<ORange>;
        using InnerVal = std::ranges::range_value_t<IRange>;
        using ResType = decltype(resSelector(std::declval<OuterVal>(), std::declval<InnerVal>()));

        std::vector<ResType> res;
        for (const auto& outerElem: outer) {
            auto outerKey = outerKeySel(outerElem);
            for (const auto& innerElem: inner) {
                if (outerKey == innerKeyS(innerElem)) {
                    res.push_back(resSelector(outerElem, innerElem));
                }
            }
        }

        return res;
    }
} // namespace linq
