#pragma once

#include "core/math/point2d.hpp"
#include "core/math/aabb.hpp"

#include <vector>

namespace generation::pipeline
{
    std::vector<math::Point2Dd> samplePoints(uint64_t seed, math::AABB bounds, int numPoints);
}
