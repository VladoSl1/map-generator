#pragma once

#include <raylib.h>

#include "core/math/math.hpp"

namespace renderer
{
    template <typename T>
    inline Vector2 toRaylib(const math::Point2D<T>& point)
    {
        return {static_cast<float>(point.x), static_cast<float>(point.y)};
    }
}
