#pragma once

#include <iterator>

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
    };

}
