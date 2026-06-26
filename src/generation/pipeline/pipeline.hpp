#pragma once

#include "core/config.hpp"
#include "core/math/math.hpp"
#include "core/utils/random.hpp"
#include "generation/pipeline/point_sampling.hpp"

namespace generation
{
    class GenerationPipeline
    {
    public:
        GenerationPipeline(int width, int height, int worldSeed)
            : m_width(width), m_height(height), m_worldSeed(worldSeed)
        {
        }

        std::vector<math::Point2D> points;


        void generate();

    private:
        int m_width, m_height, m_worldSeed;
    };
}
