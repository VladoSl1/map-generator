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
    void bucketSortPrimitives(std::vector<math::IndexPrimitive<N>>& primitives, size_t vertexIndex);

    /* alg: https://en.wikipedia.org/wiki/Radix_sort */
    template<size_t N>
    void sortPrimitives(std::vector<math::IndexPrimitive<N>>& primitive);
}
