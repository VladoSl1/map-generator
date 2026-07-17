#pragma once
#include "point2d.hpp"
#include "interval.hpp"


namespace math
{
    struct AABB
    {
        Interval<double> x_bounds;
        Interval<double> y_bounds;

        inline constexpr bool contains(const Point2Dd& p) const
        {
            return x_bounds.contains(p.x) && y_bounds.contains(p.y);
        }
    };
}
