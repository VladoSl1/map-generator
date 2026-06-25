#include "renderer.hpp"

#include "core/config.hpp"


namespace renderer
{
    void renderPoints(const std::vector<math::Point2D>& points)
    {
        for (const auto& point : points)
        {
            DrawCircle(point.x, point.y, config::renderer::POINT_RADIUS, RED);
        }
    }
}
