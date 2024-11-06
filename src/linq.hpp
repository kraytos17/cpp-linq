#pragma once

#include <algorithm>
#include <ranges>
#include <utility>
#include <vector>

namespace linq {
    template<std::ranges::input_range Range, typename Pred>
    constexpr auto where(Range&& range, Pred&& pred) {
        return std::ranges::ref_view{range} | std::views::filter(std::forward<Pred>(pred));
    }

    template<std::ranges::input_range Range, typename Proj>
    constexpr auto select(Range&& range, Proj&& proj) {
        return std::ranges::ref_view{range} | std::views::transform(std::forward<Proj>(proj));
    }

    template<std::ranges::input_range Range, typename KeySelector>
    constexpr auto orderBy(Range&& range, KeySelector&& ks) {
        using T = std::ranges::range_value_t<Range>;

        auto sorted = std::vector<T>(range.begin(), range.end());
        std::ranges::sort(sorted, {}, std::forward<KeySelector>(ks));

        return sorted;
    }


    template<std::ranges::input_range Range>
    constexpr auto toVector(Range&& range) {
        using T = std::ranges::range_value_t<Range>;
        return std::vector<T>(range.begin(), range.end());
    }
} // namespace linq
