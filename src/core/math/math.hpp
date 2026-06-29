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
            return indices.at(index);
        }

        size_t& operator[](size_t index)
        {
            return indices.at(index);
        }
    };

    using EdgeI     = IndexPrimitive<2>;
    using TriangleI = IndexPrimitive<3>;

    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    Point2Dd calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C)
    {
        double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
        if (D == 0)
        {
            throw std::runtime_error("Points are collinear; circumcenter is undefined.");
         }

        double Ux = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
                     (B.x * B.x + B.y * B.y) * (C.y - A.y) +
                     (C.x * C.x + C.y * C.y) * (A.y - B.y)) / D;

        double Uy = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
                     (B.x * B.x + B.y * B.y) * (A.x - C.x) +
                     (C.x * C.x + C.y * C.y) * (B.x - A.x)) / D;

        return {Ux, Uy};

    }
}
