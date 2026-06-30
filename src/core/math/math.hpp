#pragma once

#include <algorithm>
#include <array>
#include <vector>

namespace math
{
    template<typename T>
    struct Point2D
    {
        T x;
        T y;

        template<typename U>
        Point2D<U> cast() const {
            return { static_cast<U>(x), static_cast<U>(y) };
        }
    };


    using Point2Di = Point2D<int>;
    using Point2Df = Point2D<float>;
    using Point2Dd = Point2D<double>;

    template <size_t N>
    struct IndexPrimitive
    {
        std::array<size_t, N> indices;

        void sort_indices()
        {
            std::sort(indices.begin(), indices.end());
        }

        size_t operator[](size_t index) const
        {
            return indices[index];
        }

        size_t& operator[](size_t index)
        {
            return indices[index];
        }
    };

    using EdgeI     = IndexPrimitive<2>;
    using TriangleI = IndexPrimitive<3>;

    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    Point2Dd calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);

    bool shareEdge(const TriangleI& triangleA, const TriangleI& triangleB);

    template<size_t N>
    void bucketSortPrimitives(std::vector<math::IndexPrimitive<N>>& primitives, size_t vertexIndex)
    {
        const int n = primitives.size();
        if (n <= 1) return;

        std::vector<size_t> counts(n, 0);

        // count how many times each vertex value appears
        for (const auto& primitive : primitives)
        {
            counts[primitive[vertexIndex]]++;
        }

        // convert counts to prefix sums to determine the final index positions
        for (size_t i = 1; i < n; ++i)
        {
            counts[i] += counts[i - 1];
        }

        // build the sorted output array
        std::vector<math::IndexPrimitive<N>> output(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i)
        {
            size_t val = primitives[i][vertexIndex];
            output[counts[val] - 1] = primitives[i];
            counts[val]--;
        }

        primitives = std::move(output);
    }

    template<size_t N>
    void sortPrimitives(std::vector<math::IndexPrimitive<N>>& primitives)
    {
        for (int vertexIndex = N-1; vertexIndex >= 0; --vertexIndex)
        {
            bucketSortPrimitives(primitives, vertexIndex);
        }
    }


    template<size_t N>
    std::array<EdgeI, N> convertToEdges(const math::IndexPrimitive<N>& primitive)
    {
        std::array<EdgeI, N> edges;

        for (size_t i = 0; i < N; ++i)
        {
            size_t v1 = primitive[i];
            size_t v2 = primitive[i + 1 == N ? 0 : i + 1];

            edges[i] = EdgeI{{ std::min(v1, v2), std::max(v1, v2) }};
        }

        return edges;
    }

}
