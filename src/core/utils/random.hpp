#pragma once
#include <random>


namespace math
{
    using RngEngine = std::mt19937;
    using UnifIntDistribution = std::uniform_int_distribution<int>;
    using UnifFloatDistribution = std::uniform_real_distribution<float>;


    inline float getRandomFloat(RngEngine& engine, UnifFloatDistribution& range)
    {
        return range(engine);
    }

    inline int getRandomInt(RngEngine& engine, UnifIntDistribution& range)
    {
        return range(engine);
    }

}
