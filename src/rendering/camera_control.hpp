#pragma once

#include <raylib.h>

#include "core/math/aabb.hpp"

namespace renderer
{
    class CameraController
    {
    public:
        // TODO: screenWidth and screenHeight are not used, consider removing them
        CameraController(int screenWidth, int screenHeight);

        void update();


        math::AABB getViewBounds() const;

        const Camera2D& getCamera() const;

    private:
        Camera2D camera = { 0 };
    };
}
