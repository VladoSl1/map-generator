#pragma once

#include "core/math/math.hpp"

#include <vector>

namespace generation
{
    class GenerationPipeline
    {
    public:

        std::vector<math::Point2Di> points;
        std::vector<math::TriangleI> triangles;

        GenerationPipeline(int width, int height, int worldSeed)
            : m_width(width), m_height(height), m_worldSeed(worldSeed) {}

        void generate();

    private:
        int m_width, m_height, m_worldSeed;
    };
}
