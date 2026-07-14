#pragma once

#include <vector>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"


namespace generation::pipeline
{
    std::vector<math::TriangleI> triangulate(const std::vector<math::Point2Dd>& points);
}

