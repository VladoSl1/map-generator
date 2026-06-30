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
            triangles.push_back(math::TriangleI{d.triangles[i], d.triangles[i + 1], d.triangles[i + 2]});
            triangles.back().sort_indices();
        }

        return triangles;
    }

    void bucketSortTriangles(std::vector<math::TriangleI>& triangles, size_t vertexIndex)
    {
        const int n = triangles.size();
        if (n <= 1) return;

        std::vector<size_t> counts(n, 0);

        // count how many times each vertex value appears
        for (const auto& triangle : triangles)
        {
            counts[triangle[vertexIndex]]++;
        }

        // convert counts to prefix sums to determine the final index positions
        for (size_t i = 1; i < n; ++i)
        {
            counts[i] += counts[i - 1];
        }

        // build the sorted output array
        std::vector<math::TriangleI> output(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i)
        {
            size_t val = triangles[i][vertexIndex];
            output[counts[val] - 1] = triangles[i];
            counts[val]--;
        }

        triangles = std::move(output);
    }

    /* alg: https://en.wikipedia.org/wiki/Radix_sort */
    void sortTriangles(std::vector<math::TriangleI>& triangles)
    {
        for (int vertexIndex = 2; vertexIndex >= 0; --vertexIndex)
        {
            bucketSortTriangles(triangles, vertexIndex);
        }
    }
}
