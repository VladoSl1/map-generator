#pragma once
#include <random>


namespace math
{

    inline float getRandomFloat(std::mt19937& engine, float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }

    inline int getRandomInt(std::mt19937& engine, int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

}
