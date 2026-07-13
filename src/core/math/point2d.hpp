#pragma once

#include <algorithm>

namespace math
{

    template<typename T>
    struct Point2D
    {
        T x;
        T y;

        template<typename U>
        Point2D<U> cast() const
        {
            // apply special rounding and clamping when casting from float/double to int/size_t
            if constexpr (std::is_floating_point_v<T> && std::is_integral_v<U>)
            {
                T rounded_x = std::round(x);
                T rounded_y = std::round(y);

                constexpr T min_limit = static_cast<T>(std::numeric_limits<U>::min());
                constexpr T max_limit = static_cast<T>(std::numeric_limits<U>::max());

                rounded_x = std::clamp(rounded_x, min_limit, max_limit);
                rounded_y = std::clamp(rounded_y, min_limit, max_limit);

                return { static_cast<U>(rounded_x), static_cast<U>(rounded_y) };
            }
            else
            {
                // default fallback for safe casts (e.g., int -> float, float -> double)
                return { static_cast<U>(x), static_cast<U>(y) };
            }
        }

        Point2D& operator+=(const Point2D& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Point2D& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Point2D& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }
    };

    template<typename T>
    inline Point2D<T> operator+(Point2D<T> lhs, const Point2D<T>& rhs) {
        lhs += rhs;
        return lhs;
    }

    template<typename T>
    inline Point2D<T> operator*(Point2D<T> point, T scalar) {
        point *= scalar;
        return point;
    }


    using Point2Di = Point2D<int>;
    using Point2Df = Point2D<float>;
    using Point2Dd = Point2D<double>;
}
