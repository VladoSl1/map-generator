#pragma once

#include "generation/pipeline/voronoi_diagram.hpp"

#include <vector>

namespace generation::pipeline
{
    VoronoiDiagram generate(uint64_t seed, int width, int height);
    VoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> points);
}
