#include "renderer.hpp"

#include "core/config.hpp"

#include "raylib_adapters.hpp"

namespace renderer
{
    void renderPoints(const std::vector<math::Point2Dd>& points, Color color)
    {
        for (const auto& point : points)
        {
            DrawCircle(point.x, point.y, config::renderer::POINT_RADIUS, color);
        }
    }

    void renderTriangles(const std::vector<math::Point2Dd>& points,
                         const std::vector<math::TriangleI>& triangles,
                         Color color)
    {
        for (const auto& triangle : triangles)
        {
            DrawTriangleLines(toRaylib(points[triangle[0]]),
                              toRaylib(points[triangle[1]]),
                              toRaylib(points[triangle[2]]),
                              color);
        }
    }

    void renderEdges(const std::vector<math::Point2Dd>& points,
                     const std::vector<math::EdgeI>& edges,
                     Color color)
    {
        for (const auto& edge : edges)
        {
            DrawLineV(toRaylib(points[edge[0]]),
                      toRaylib(points[edge[1]]),
                      color);
        }
    }
}
