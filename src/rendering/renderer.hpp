#include <raylib.h>

#include "core/math/math.hpp"

#include <vector>


namespace renderer
{
    void renderPoints(const std::vector<math::Point2Di>& points);
    void renderTriangles(const std::vector<math::Point2Di>& points,
                         const std::vector<math::TriangleI>& triangles);

}
