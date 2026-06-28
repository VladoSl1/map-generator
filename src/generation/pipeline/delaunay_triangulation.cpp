#include "delaunay_triangulation.hpp"

#include <delaunator.hpp>


namespace generation::pipeline
{
    std::vector<math::TriangleI> triangulate(const std::vector<math::Point2Di>& points)
    {
        // flatten the points into a single vector of coordinates x0, y0, x1, y1, ...
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
            triangles.emplace_back(d.triangles[i], d.triangles[i + 1], d.triangles[i + 2]);
            triangles.back().sort_indices();
        }

        return triangles;
    }

    void bucketSortTriangles(std::vector<math::TriangleI>& triangles, size_t vertexIndex)
    {
        std::vector<std::vector<math::TriangleI>> buckets(triangles.size());

        for (const auto& triangle : triangles)
        {
            buckets[triangle[vertexIndex]].push_back(triangle);
        }

        // Clear the original vector and concatenate the buckets back into it
        triangles.clear();
        for (const auto& bucket : buckets)
        {
            triangles.insert(triangles.end(), bucket.begin(), bucket.end());
        }
    }

    void sortTriangles(std::vector<math::TriangleI>& triangles)
    {
        for (int vertexIndex = 2; vertexIndex >= 0; --vertexIndex)
        {
            bucketSortTriangles(triangles, vertexIndex);
        }
    }
}
