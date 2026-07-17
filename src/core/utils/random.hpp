#pragma once

#include <random>
#include <type_traits>

#include "core/math/interval.hpp"

namespace utils
{
    using RngEngine = std::mt19937;

    // this is ment to be used outside of the loop, so that the distribution is constructed only once, and not for every iteration
    template <typename T>
    inline auto makeDistribution(const math::Interval<T>& interval)
    {
        if constexpr (std::is_integral_v<T>)
        {
            return std::uniform_int_distribution<T>(interval.min, interval.max);
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            return std::uniform_real_distribution<T>(interval.min, interval.max);
        }
        else
        {
            static_assert(std::is_arithmetic_v<T>, "Unsupported type for random distribution.");
        }
    }

    // convenience function to get a random value from an interval using the provided engine
    template <typename T>
    inline T getRandom(RngEngine& engine, const math::Interval<T>& interval)
    {
        auto dist = makeDistribution(interval);
        return dist(engine);
    }
}
