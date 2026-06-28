#pragma once

#include <iterator>
#include <vector>

namespace math
{
    template<typename T>
    struct Point2D
    {
        T x;
        T y;
    };


    using Point2Di = Point2D<int>;
    using Point2Df = Point2D<float>;
    using Point2Dd = Point2D<double>;

    struct TriangleI
    {
        size_t a;
        size_t b;
        size_t c;

        void sort_indices()
        {
            if (a > b) std::swap(a, b);
            if (b > c) std::swap(b, c);
            if (a > b) std::swap(a, b);
        }

        size_t operator[](size_t index) const
        {
            switch (index)
            {
            case 0: return a;
            case 1: return b;
            case 2: return c;
            default: throw std::out_of_range("TriangleI index out of range");
            }
        }
    };

    struct Polygon
    {
        std::vector<Point2Di> vertices;
    };

}
