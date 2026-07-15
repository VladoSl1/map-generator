#pragma once
#include <random>


namespace math
{
    using RngEngine = std::mt19937;
    using UnifIntDistribution = std::uniform_int_distribution<int>;
    using UnifFloatDistribution = std::uniform_real_distribution<float>;
    using UnifDoubleDistribution = std::uniform_real_distribution<double>;


    inline float getRandomDouble(RngEngine& engine, UnifDoubleDistribution& range)
    {
        return range(engine);
    }

    inline float getRandomFloat(RngEngine& engine, UnifFloatDistribution& range)
    {
        return range(engine);
    }

    inline int getRandomInt(RngEngine& engine, UnifIntDistribution& range)
    {
        return range(engine);
    }

}
