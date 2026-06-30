#pragma once

#include "core/math/math.hpp"

#include "generation/pipeline/voronoi_diagram.hpp"

#include <vector>

namespace generation
{
    class GenerationPipeline
    {
    public:

        std::vector<math::Point2Di> points;
        std::vector<math::TriangleI> triangles;
        std::vector<math::Point2Di> voronoiVertices;

        pipeline::VoronoiDiagram voronoiDiagram;

        GenerationPipeline(int width, int height, int worldSeed)
            : m_width(width), m_height(height), m_worldSeed(worldSeed) {}

        void generate();

    private:
        int m_width, m_height, m_worldSeed;
    };
}
