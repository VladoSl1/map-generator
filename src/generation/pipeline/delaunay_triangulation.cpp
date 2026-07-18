#include "delaunay_triangulation.hpp"

#include <delaunator.hpp>

#include <cassert>

namespace generation::pipeline
{
    std::vector<math::TriangleI> triangulate(const std::vector<math::Point2Dd>& points)
    {
        assert(points.size() >= 3 && "At least 3 points are required for triangulation");

        // flatten the points into a single vector of coordinates x0, y0, x1, y1, ...
        // double type because delaunator expects double coordinates
        std::vector<double> coords;
        coords.reserve(points.size() * 2);

        for (const auto& point : points)
        {
            coords.push_back(point.x);
            coords.push_back(point.y);
        }

        delaunator::Delaunator d(coords);

        std::vector<math::TriangleI> triangles;
        triangles.reserve(d.triangles.size() / 3);
        for (size_t i = 0; i < d.triangles.size(); i += 3)
        {
            triangles.push_back({ d.triangles[i], d.triangles[i + 1], d.triangles[i + 2] });
            triangles.back().sort_indices();
        }

        return triangles;
    }
}
