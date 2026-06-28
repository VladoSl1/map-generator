#pragma once

#include "core/math/math.hpp"

#include <vector>

namespace generation::pipeline
{
    std::vector<math::TriangleI> triangulate(const std::vector<math::Point2Di>& points);

    void sortTriangles(std::vector<math::TriangleI>& triangles);
}

