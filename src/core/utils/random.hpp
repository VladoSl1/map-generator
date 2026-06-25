#pragma once
#include <random>


namespace math
{
    using RngEngine = std::mt19937;
    using UnifIntDistribution = std::uniform_int_distribution<int>;
    using UnifFloatDistribution = std::uniform_real_distribution<float>;


    inline float getRandomFloat(RngEngine& engine, float min, float max)
    {
        UnifFloatDistribution dist(min, max);
        return dist(engine);
    }

    inline int getRandomInt(RngEngine engine, int min, int max)
    {
        UnifIntDistribution dist(min, max);
        return dist(engine);
    }

}
