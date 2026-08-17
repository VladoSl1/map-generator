#pragma once

#include <raylib.h>

#include "core/math/aabb.hpp"

namespace renderer
{
    class CameraController
    {
    public:
        CameraController();

        void update();


        math::AABB getViewBounds() const;

        const Camera2D& getCamera() const;

    private:
        Camera2D camera = { 0 };
        float screenWidth, screenHeight;
    };
}
