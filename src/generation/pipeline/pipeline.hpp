#pragma once

#include "generation/pipeline/voronoi_diagram.hpp"

#include "core/math/aabb.hpp"
#include "core/math/point2d.hpp"

#include <cstdint>
#include <vector>

namespace generation::pipeline
{
    DynamicVoronoiDiagram generateFromPoints(std::vector<math::Point2Dd> points);
}
