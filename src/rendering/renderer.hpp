#include <raylib.h>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

#include <vector>


namespace renderer
{
    void renderPoints(const std::vector<math::Point2Di>& points, Color color);
    void renderTriangles(const std::vector<math::Point2Di>& points,
                         const std::vector<math::TriangleI>& triangles,
                         Color color = GREEN);
    void renderEdges(const std::vector<math::Point2Di>& points,
                     const std::vector<math::EdgeI>& edges,
                     Color color);
}
