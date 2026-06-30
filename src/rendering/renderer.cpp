#include "renderer.hpp"

#include "core/config.hpp"

#include "raylib_adapters.hpp"

namespace renderer
{
    void renderPoints(const std::vector<math::Point2Di>& points)
    {
        for (const auto& point : points)
        {
            DrawCircle(point.x, point.y, config::renderer::POINT_RADIUS, RED);
        }
    }

    void renderTriangles(const std::vector<math::Point2Di>& points,
                         const std::vector<math::TriangleI>& triangles)
    {
        for (const auto& triangle : triangles)
        {
            DrawTriangleLines(toRaylib(points[triangle[0]]),
                              toRaylib(points[triangle[1]]),
                              toRaylib(points[triangle[2]]),
                              GREEN);
        }
    }
}
