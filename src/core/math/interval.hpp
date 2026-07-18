#pragma once

#include <algorithm>

namespace math
{
    template <typename T>
    struct Interval
    {
        T min, max;

        constexpr T length() const { return max - min; }
        constexpr bool contains(T value) const { return value >= min && value <= max; }
        constexpr T clamp(T value) const { return std::clamp(value, min, max); }
    };
}
